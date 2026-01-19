#include "grass.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>

namespace Example {
    Grass::Grass() : TexturedShape() {}
    Grass::~Grass() {}

    void Grass::initGrassField(GLuint tex, float areaSize, float step) {
        this->textureID = tex;

        if (!shaderReady) {
            compileShader();
        }

                float h = 0.8f, w = 0.4f;
        std::vector<TexturedVertex> v = {
            {{-w, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{ w, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{ 0.0f, h, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 1.0f}},

            {{0.0f, 0.0f, -w}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.0f, 0.0f,  w}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.0f,  h, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 1.0f}}
        };
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

        float startX = 10.0f;
        float endX = 15.0f;
        float startZ = 70.5f;
        float endZ = 150.0f;

        for (float x = -areaSize; x < areaSize; x += step) {
            for (float z = -areaSize; z < areaSize; z += step) {

                float absX = std::abs(x);
                bool onSidewalk = (absX > startX && absX < endX);
                bool alongPath = (z > startZ && z < endZ);

                if (onSidewalk && alongPath) {
                    float offX = (rand() % 100 / 300.0f);
                    float offZ = (rand() % 100 / 300.0f);
                    this->instancePositions.push_back(glm::vec3(x + offX, 0.01f, z + offZ));
                }
            }
        }
    }

    void Grass::draw(const glm::mat4& cameraProj) const {
        this->renderField(cameraProj);
    }
}
