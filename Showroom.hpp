#pragma once
#include "Room.hpp"
#include "Car.hpp"
#include "Lighting.hpp"  // ✅ جديد
#include <vector>
#include <memory>

namespace Example
{
    struct ColumnBounds
    {
        float xCenter, zCenter;
        float halfSize;
    };

    struct ExteriorWallBounds
    {
        float minX, maxX;
        float minZ, maxZ;
        float doorMinX, doorMaxX;
        float doorZ;
    };

    class Showroom
    {
    private:
        std::vector<std::unique_ptr<Room>> rooms;
        Example::BasicShape outerGround;
        Example::BasicShape ceiling;
        Example::BasicShape receptionDesk;
        Example::BasicShape columns;

        Example::BasicShape streetAsphalt;
        Example::BasicShape sidewalk;
        Example::BasicShape streetLines;

        std::vector<ColumnBounds> columnBounds;

        Example::BasicShape exteriorWalls;
        Example::BasicShape windowFrames;
        Example::BasicShape windowGlass;
        Example::BasicShape mainDoorFrame;

        ExteriorWallBounds exteriorBounds;

    public:
        // ✅ جديد: مدير الإضاءة (public للوصول إليه)
        LightingManager lighting;

        void init();
        void renderAll(const glm::mat4& viewProj);
        glm::vec3 checkCollision(glm::vec3 currentPos, glm::vec3 nextPos);
        Car* findNearestCar(glm::vec3 playerPos);

        // ✅ جديد: تبديل الإضاءة
        void toggleLights();
    };
}