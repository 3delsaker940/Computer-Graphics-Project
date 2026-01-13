#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example {
    class Room {
    public:
        std::string name;
        Example::BasicShape floor;
        Example::BasicShape walls;
        Example::BasicShape podium;       // منصة السيارة
        Example::BasicShape ceilingLight; // إضاءة السقف

        glm::vec3 centerOffset;
        float size;
        bool doorAtMaxZ;

        Room() : size(0), centerOffset(0.0f), doorAtMaxZ(true) {}

        Room(Room&& other) noexcept = default;
        Room& operator=(Room&& other) noexcept = default;
        Room(const Room&) = delete;
        Room& operator=(const Room&) = delete;

        void create(std::string name, glm::vec3 position, float size, glm::vec3 color, bool isDoorAtMaxZ);
        void draw(const glm::mat4& viewProjMatrix);
    };
}