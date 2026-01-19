#pragma once
#include "textured-shape.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace Example {
    class Grass : public TexturedShape {
    public:
        Grass();
        ~Grass();

        void initGrassField(GLuint tex, float areaSize, float step);
        void setFloorTexture(GLuint tex) { floorTextureID = tex; }
        void draw(const glm::mat4& cameraProj) const;

    private:
        GLuint floorVAO, floorVBO;
        GLuint floorTextureID;
        int floorVertexCount;
    };
}
