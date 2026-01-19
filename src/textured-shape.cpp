#include "textured-shape.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Example {
  
    GLuint TexturedShape::shaderProgram = 0;
    GLuint TexturedShape::cameraLoc = 0, TexturedShape::transformLoc = 0, TexturedShape::timeLoc = 0;
    bool TexturedShape::shaderReady = false;

        static const char* vertexSrc = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;
uniform mat4 transform;
uniform mat4 camera;
uniform float time;
out vec2 vUV;
void main() {
    vUV = aUV;
    vec3 pos = aPos;
        if(pos.y > 0.1) { 
       pos.x += sin(time * 3.0 + abs(transform[3][0]) + transform[3][2]) * 0.25;
    }
    gl_Position = camera * transform * vec4(pos, 1.0);
})";

    static const char* fragmentSrc = R"(#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D ourTexture;
void main() {
    vec4 texColor = texture(ourTexture, vUV);
        if(texColor.a < 0.2) discard; 
    FragColor = texColor;
})";

    void TexturedShape::compileShader() {
        if (shaderReady) return;

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexSrc, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentSrc, nullptr);
        glCompileShader(fs);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        cameraLoc = glGetUniformLocation(shaderProgram, "camera");
        transformLoc = glGetUniformLocation(shaderProgram, "transform");
        timeLoc = glGetUniformLocation(shaderProgram, "time");
        shaderReady = true;
    }

    void TexturedShape::renderField(const glm::mat4& camera) const {
        if (instancePositions.empty() || VAO == 0) return;

        glUseProgram(shaderProgram);

                glUniform1f(timeLoc, (float)glfwGetTime());

                glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

                glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera));

        glBindVertexArray(VAO);

                for (const auto& pos : instancePositions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }

        glBindVertexArray(0);
    }

  
    TexturedShape::TexturedShape() {}
    TexturedShape::~TexturedShape() {
    
    }

    TexturedShape::TexturedShape(TexturedShape&& other) noexcept {
        *this = std::move(other);
    }

    TexturedShape& TexturedShape::operator=(TexturedShape&& other) noexcept {
        if (this != &other) {
            this->VAO = other.VAO; other.VAO = 0;
            this->VBO = other.VBO; other.VBO = 0;
            this->textureID = other.textureID;
            this->vertexCount = other.vertexCount;
            this->instancePositions = std::move(other.instancePositions);
        }
        return *this;
    }
}