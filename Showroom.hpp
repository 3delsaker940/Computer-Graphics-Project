#pragma once
#include "Room.hpp"
#include "Car.hpp"
#include "Lighting.hpp"  
#include <vector>
#include <memory>
#include "SportsCar.hpp"  
#include "SUVCar.hpp" 
#include "ElectricCar.hpp"

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


        ExteriorWallBounds exteriorBounds;      // حدود التصادم
        BasicShape streetLamps;      // أعمدة الإنارة

        //مشان الدوار
        Example::BasicShape roundabout;

        SportsCar sportsCarInRoom;      // للغرفة الصفراء (Sports)
        SUVCar suvCarInRoom;            // للغرفة الخضراء (Family)
        ElectricCar electricCarInRoom;


    public:
        LightingManager lighting;

        void init();
        void renderAll(const glm::mat4& viewProj);
        glm::vec3 checkCollision(glm::vec3 currentPos, glm::vec3 nextPos);
        Car* findNearestCar(glm::vec3 playerPos);
        void toggleLights();
        void update(float dt);
        bool toggleNearestRoomDoor(const glm::vec3& playerPos);
        glm::vec3 checkCollisionRadius(glm::vec3 currentPos, glm::vec3 nextPos, float radius);
    };
}