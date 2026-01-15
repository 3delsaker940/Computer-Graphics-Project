#pragma once
#include "Room.hpp"
#include <vector>
#include <memory>

namespace Example {
    class Showroom {
    private:
        std::vector<std::unique_ptr<Room>> rooms;
        Example::BasicShape outerGround;
        Example::BasicShape ceiling;       // السقف العام
        Example::BasicShape receptionDesk;
        Example::BasicShape columns;       // الأعمدة

        Example::BasicShape streetAsphalt; // الزفت
        Example::BasicShape sidewalk;      // الرصيف
        Example::BasicShape streetLines;
    public:
        void init();
        void renderAll(const glm::mat4& viewProj);
        glm::vec3 checkCollision(glm::vec3 currentPos, glm::vec3 nextPos);
    };
}