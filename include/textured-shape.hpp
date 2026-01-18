#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Example {
    struct TexturedVertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 uv;
    };

    class TexturedShape {
    protected: //new
        GLuint VAO = 0, VBO = 0;
        int vertexCount = 0;
        GLuint textureID = 0;

        static GLuint shaderProgram;
        static GLuint cameraLoc, transformLoc, timeLoc;
        static bool shaderReady;

        static void compileShader();

    public:
        std::vector<glm::vec3> instancePositions; //new

        TexturedShape();
        virtual ~TexturedShape();

        void renderField(const glm::mat4& camera) const;

        TexturedShape(TexturedShape&& other) noexcept;
        TexturedShape& operator=(TexturedShape&& other) noexcept;
        TexturedShape(const TexturedShape&) = delete;
        TexturedShape& operator=(const TexturedShape&) = delete;
    };
}