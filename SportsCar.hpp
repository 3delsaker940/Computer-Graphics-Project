#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    class SportsCar
    {
    private:
        // الهيكل الخارجي
        BasicShape body;
        BasicShape hood;
        BasicShape roof;
        BasicShape rear;
        BasicShape doors;          // ✅ الأبواب

        // العجلات
        BasicShape wheels;
        BasicShape wheelRims;

        // المصابيح
        BasicShape headlights;
        BasicShape taillights;

        // النوافذ
        BasicShape windowFront;
        BasicShape windowRear;
        BasicShape windowsSide;    // نوافذ الأبواب

        // التفاصيل
        BasicShape grille;
        BasicShape mirrors;
        BasicShape spoiler;
        BasicShape doorHandles;    // ✅ مقابض الأبواب

        glm::vec3 position;
        float rotation;
        glm::vec3 bodyColor;

        // ✅ أبعاد أكبر
        float length = 4.6f;       // كان 4.2
        float width = 2.1f;        // كان 1.95
        float height = 1.25f;      // كان 1.15
        float groundClearance = 0.15f;  // كان 0.12

        void buildBody();
        void buildDoors();         // ✅ جديد
        void buildWheels();
        void buildLights();
        void buildWindows();
        void buildDetails();

    public:
        SportsCar();
        ~SportsCar() = default;

        void create(glm::vec3 pos, float rotationY = 0.0f,
            glm::vec3 color = { 0.9f, 0.1f, 0.1f });

        void render(const glm::mat4& viewProj);

        glm::vec3 getPosition() const { return position; }
        float getRotation() const { return rotation; }
    };
}