#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "basic-shape.hpp"
#include "Car.hpp"  
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

        std::vector<Example::BasicShape> decorations;
        std::vector<Example::BasicShape> exteriorDecor; 
        Example::BasicShape backWallWindowFrame;
        Example::BasicShape backWallWindowGlass;
        bool hasBackWallWindow = false;

    
        float doorWidth = 7.0f;           float doorHeight = 4.5f;  
        glm::vec3 centerOffset;
        float size;
        bool doorAtMaxZ;

        std::vector<PodiumBounds> podiumBounds;

                std::vector<std::unique_ptr<Car>> cars;

        Room() : size(0), centerOffset(0.0f), doorAtMaxZ(true) {}

        Room(Room&& other) noexcept = default;
        Room& operator=(Room&& other) noexcept = default;
        Room(const Room&) = delete;
        Room& operator=(const Room&) = delete;

        void create(std::string name, glm::vec3 position, float size, glm::vec3 color, bool isDoorAtMaxZ);
        void draw(const glm::mat4& viewProjMatrix);

                void addCarToPodium(int podiumIndex, glm::vec3 carColor, glm::vec3 interiorColor = { 0.15f, 0.12f, 0.1f });

        void addExistingCarToPodium(int podiumIndex, Example::Car* car);

                Car* getCarNearPlayer(glm::vec3 playerPos);

                bool doorOpen = true;                  float doorAngle = 0.0f;                float doorTargetAngle = 0.0f;          Example::BasicShape doorPanel; 
        void update(float dt);
        void toggleDoor();
        bool isNearDoor(const glm::vec3& playerPos) const;
        bool isDoorOpen() const { return doorOpen; }
    };
}