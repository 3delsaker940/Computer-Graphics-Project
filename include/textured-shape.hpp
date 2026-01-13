#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Example
{
    struct TexturedVertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 uv;
    };

    class TexturedShape
    {
    protected:
        GLuint VAO = 0;
        GLuint VBO = 0;
        int vertexCount = 0;

        static GLuint shaderProgram;
        static GLuint cameraLoc;
        static GLuint transformLoc;
        static bool shaderReady;

        static void compileShader();

    public:
        TexturedShape();
        TexturedShape(const std::vector<TexturedVertex>& vertices);
        ~TexturedShape();

        TexturedShape(TexturedShape&& other) noexcept;
        TexturedShape& operator=(TexturedShape&& other) noexcept;

        void render(const glm::mat4& model,
            const glm::mat4& camera) const;
    };
}