#include "tree.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <GLFW/glfw3.h>

namespace Example {
    Tree::Tree() : TexturedShape(), textureID2(0) {}
    Tree::~Tree() {}

    void Tree::initTrees(GLuint tex, GLuint texHuge) {
        this->textureID = tex;
        this->textureID2 = texHuge;
        if (!shaderReady) { compileShader(); }

        //شكل الشجرة
        float h = 3.0f; float w = 1.5f;
        std::vector<TexturedVertex> v;
        for (int i = 0; i < 3; i++) {
            float angle = glm::radians(i * 60.0f);
            float x = cos(angle) * w; float z = sin(angle) * w;
            v.push_back({ {-x, 0.0f, -z}, {1,1,1}, {0,0} });
            v.push_back({ { x, 0.0f,  z}, {1,1,1}, {1,0} });
            v.push_back({ { x,    h,  z}, {1,1,1}, {1,1} });
            v.push_back({ {-x, 0.0f, -z}, {1,1,1}, {0,0} });
            v.push_back({ { x,    h,  z}, {1,1,1}, {1,1} });
            v.push_back({ {-x,    h, -z}, {1,1,1}, {0,1} });
        }
        this->vertexCount = static_cast<int>(v.size());
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(TexturedVertex), v.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, uv));
        glEnableVertexAttribArray(2);

        // توزيع الشجر الصغير
        this->instancePositions.clear();
        for (float z = 80.0f; z <= 150.0f; z += 20.0f) {
            this->instancePositions.push_back(glm::vec3(12.0f, 0.0f, z));
            this->instancePositions.push_back(glm::vec3(-12.0f, 0.0f, z));
        }
        // توزيع الشجر الكبير 
        this->hugeTreePositions.clear();
      
        for (float z = 80.0f; z <= 140.0f; z += 20.0f) {
            this->hugeTreePositions.push_back(glm::vec3(45.0f, 0.0f, z));
            this->hugeTreePositions.push_back(glm::vec3(-45.0f, 0.0f, z));
        }
    }

    void Tree::draw(const glm::mat4& cameraProj) const {
        glUseProgram(shaderProgram);
        GLint colorLoc = glGetUniformLocation(shaderProgram, "overlayColor");
        glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraProj));
        glUniform1f(timeLoc, (float)glfwGetTime() * 0.01f);

        glBindVertexArray(this->VAO);

        // رسم الشجر الصغير
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        for (const auto& pos : instancePositions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }

        // رسم الشجر الكبير
        glBindTexture(GL_TEXTURE_2D, this->textureID2);
        for (size_t i = 0; i < hugeTreePositions.size(); i++) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), hugeTreePositions[i]);
            model = glm::rotate(model, (float)i * 0.5f, glm::vec3(0, 1, 0));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
    }
}