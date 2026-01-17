#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    class ElectricCar
    {
    private:
        BasicShape body;
        BasicShape hood;
        BasicShape roof;
        BasicShape rear;
        BasicShape doors;

        BasicShape wheels;
        BasicShape wheelRims;

        BasicShape headlights;
        BasicShape taillights;
        BasicShape ledStrip;       // ? ???? LED ????

        BasicShape windowFront;
        BasicShape windowRear;
        BasicShape windowsSide;

        BasicShape grille;         // ??? ???? (????????)
        BasicShape mirrors;
        BasicShape chargingPort;   // ? ???? ?????
        BasicShape doorHandles;

        glm::vec3 position;
        float rotation;
        glm::vec3 bodyColor;

        // ????? ??????? ?????????? (????? ?????????)
        float length = 4.8f;
        float width = 2.0f;
        float height = 1.5f;
        float groundClearance = 0.18f;

        void buildBody();
        void buildDoors();
        void buildWheels();
        void buildLights();
        void buildWindows();
        void buildDetails();

    public:
        ElectricCar();
        ~ElectricCar() = default;

        void create(glm::vec3 pos, float rotationY = 0.0f,
            glm::vec3 color = { 0.1f, 0.3f, 0.7f });  // ???? ???????

        void render(const glm::mat4& viewProj);

        glm::vec3 getPosition() const { return position; }
        float getRotation() const { return rotation; }
    };
}