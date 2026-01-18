#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    class SUVCar
    {
    private:
        // الهيكل الخارجي
        BasicShape body;
        BasicShape hood;
        BasicShape roof;
        BasicShape rear;

        // العجلات
        BasicShape wheels;
        BasicShape wheelRims;

        // المصابيح
        BasicShape headlights;
        BasicShape taillights;

        // النوافذ
        BasicShape windowFront;
        BasicShape windowRear;
        BasicShape windowsSide;

        // التفاصيل
        BasicShape grille;
        BasicShape mirrors;
        BasicShape roofRack;       // حامل سقف
        BasicShape bumpers;

        glm::vec3 position;
        float rotation;
        glm::vec3 bodyColor;

        // أبعاد SUV (طويلة وعالية)
        float length = 4.8f;
        float width = 2.0f;
        float height = 1.85f;
        float groundClearance = 0.25f;  // عالية عن الأرض

        void buildBody();
        void buildWheels();
        void buildLights();
        void buildWindows();
        void buildDetails();

    public:
        SUVCar();
        ~SUVCar() = default;

        void create(glm::vec3 pos, float rotationY = 0.0f,
            glm::vec3 color = { 0.2f, 0.25f, 0.3f });  // رمادي غامق

        void render(const glm::mat4& viewProj);

        glm::vec3 getPosition() const { return position; }
        float getRotation() const { return rotation; }
    };
}