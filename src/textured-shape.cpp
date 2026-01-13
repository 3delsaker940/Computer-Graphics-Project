#include "textured-shape.hpp"

#include <iostream>
#include <cstring>

#include <glm/gtc/type_ptr.hpp>

namespace Example
{
    GLuint TexturedShape::shaderProgram = 0;
    GLuint TexturedShape::cameraLoc = 0;
    GLuint TexturedShape::transformLoc = 0;
    bool   TexturedShape::shaderReady = false;

    // ---------------- SHADER SOURCE ----------------

    static const char* vertexSrc = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 transform;
uniform mat4 camera;

out vec3 vColor;

void main()
{
  vColor = aColor;
  gl_Position = camera * transform * vec4(aPos, 1.0);
}
)";

    static const char* fragmentSrc = R"(#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
  FragColor = vec4(vColor, 1.0);
}
)";

    // ---------------- SHADER COMPILATION ----------------

    static GLuint compile(GLenum type, const char* src)
    {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);

        GLint ok;
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            char log[1024];
            glGetShaderInfoLog(s, 1024, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << std::endl;
            std::exit(-1);
        }
        return s;
    }

    void TexturedShape::compileShader()
    {
        GLuint vs = compile(GL_VERTEX_SHADER, vertexSrc);
        GLuint fs = compile(GL_FRAGMENT_SHADER, fragmentSrc);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);

        GLint ok;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            char log[1024];
            glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
            std::cerr << "Shader link error:\n" << log << std::endl;
            std::exit(-1);
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        cameraLoc = glGetUniformLocation(shaderProgram, "camera");
        transformLoc = glGetUniformLocation(shaderProgram, "transform");

        shaderReady = true;
    }

    // ---------------- CONSTRUCTORS ----------------

    TexturedShape::TexturedShape() {}

    TexturedShape::TexturedShape(const std::vector<TexturedVertex>& vertices)
    {
        if (!shaderReady)
            compileShader();

        vertexCount = static_cast<int>(vertices.size());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(TexturedVertex),
            vertices.data(),
            GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex), (void*)0);
        glEnableVertexAttribArray(0);

        // color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, color));
        glEnableVertexAttribArray(1);

        // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, uv));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    // ---------------- MOVE ----------------

    TexturedShape::TexturedShape(TexturedShape&& o) noexcept
    {
        *this = std::move(o);
    }

    TexturedShape& TexturedShape::operator=(TexturedShape&& o) noexcept
    {
        VAO = o.VAO; o.VAO = 0;
        VBO = o.VBO; o.VBO = 0;
        vertexCount = o.vertexCount;
        return *this;
    }

    // ---------------- DESTRUCTOR ----------------

    TexturedShape::~TexturedShape()
    {
        if (VBO) glDeleteBuffers(1, &VBO);
        if (VAO) glDeleteVertexArrays(1, &VAO);
    }

    // ---------------- RENDER ----------------

    void TexturedShape::render(const glm::mat4& model,
        const glm::mat4& camera) const
    {
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
            glm::value_ptr(model));
        glUniformMatrix4fv(cameraLoc, 1, GL_FALSE,
            glm::value_ptr(camera));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }
}