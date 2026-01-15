#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "basic-shape.hpp"
#include "Car.hpp"  // ✅ إضافة

namespace Example
{
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
        std::vector<Example::BasicShape> podiums;
        Example::BasicShape ceilingLight;

        glm::vec3 centerOffset;
        float size;
        bool doorAtMaxZ;

        std::vector<PodiumBounds> podiumBounds;

        // ✅ جديد: السيارات في الغرفة
        std::vector<std::unique_ptr<Car>> cars;

        Room() : size(0), centerOffset(0.0f), doorAtMaxZ(true) {}

        Room(Room&& other) noexcept = default;
        Room& operator=(Room&& other) noexcept = default;
        Room(const Room&) = delete;
        Room& operator=(const Room&) = delete;

        void create(std::string name, glm::vec3 position, float size, glm::vec3 color, bool isDoorAtMaxZ);
        void draw(const glm::mat4& viewProjMatrix);

        // ✅ جديد: إضافة سيارة على منصة معينة
        void addCarToPodium(int podiumIndex, glm::vec3 carColor, glm::vec3 interiorColor = { 0.15f, 0.12f, 0.1f });

        // ✅ جديد: فحص إذا كان اللاعب قرب باب سيارة
        Car* getCarNearPlayer(glm::vec3 playerPos);
    };
}