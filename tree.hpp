#pragma once
#include "textured-shape.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace Example {
    class Tree : public TexturedShape {
    public:
        Tree();
        ~Tree();
        void initTrees(GLuint tex, GLuint texHuge);

        void draw(const glm::mat4& cameraProj) const;

    private:
        GLuint textureID2; 
        std::vector<glm::vec3> hugeTreePositions; 
    };
}