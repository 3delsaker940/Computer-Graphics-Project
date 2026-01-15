#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    // ✅ هيكل لتخزين حدود المنصة للتصادم
    struct PodiumBounds
    {
        float xCenter, zCenter;
        float halfWidth, halfDepth;
    };

    class Room
    {
    public:
        std::string name;
        Example::BasicShape floor;
        Example::BasicShape walls;
        std::vector<Example::BasicShape> podiums;  // ✅ 3 منصات بدلاً من واحدة
        Example::BasicShape ceilingLight;

        glm::vec3 centerOffset;
        float size;
        bool doorAtMaxZ;

        // ✅ حدود المنصات للتصادم
        std::vector<PodiumBounds> podiumBounds;

        Room() : size(0), centerOffset(0.0f), doorAtMaxZ(true) {}

        Room(Room&& other) noexcept = default;
        Room& operator=(Room&& other) noexcept = default;
        Room(const Room&) = delete;
        Room& operator=(const Room&) = delete;

        void create(std::string name, glm::vec3 position, float size, glm::vec3 color, bool isDoorAtMaxZ);
        void draw(const glm::mat4& viewProjMatrix);
    };
}