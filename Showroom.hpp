#pragma once
#include "Room.hpp"
#include <vector>
#include <memory>

namespace Example
{
    struct ColumnBounds
    {
        float xCenter, zCenter;
        float halfSize;
    };

    // ✅ جديد: حدود الجدار الخارجي
    struct ExteriorWallBounds
    {
        float minX, maxX;
        float minZ, maxZ;
        float doorMinX, doorMaxX;  // حدود الباب الرئيسي
        float doorZ;               // موقع Z للباب
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

        // ✅ جديد: الجدران الخارجية والنوافذ
        Example::BasicShape exteriorWalls;      // الجدران الخارجية
        Example::BasicShape windowFrames;       // إطارات النوافذ
        Example::BasicShape windowGlass;        // زجاج النوافذ (شفاف)
        Example::BasicShape mainDoorFrame;      // إطار الباب الرئيسي

        ExteriorWallBounds exteriorBounds;      // حدود التصادم
        BasicShape streetLamps;      // أعمدة الإنارة

        //مشان الدوار
        Example::BasicShape roundabout;

    public:
        void init();
        void renderAll(const glm::mat4& viewProj);
        glm::vec3 checkCollision(glm::vec3 currentPos, glm::vec3 nextPos);
    };
}