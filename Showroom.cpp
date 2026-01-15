#include "Showroom.hpp"
#include <iostream>

namespace Example
{
    void Showroom::init()
    {
        // 1. الأرضية الكبرى (كما كانت)
        std::vector<BasicVertex> gVerts;
        glm::vec3 gColor = {0.05f, 0.05f, 0.07f};
        float gS = 120.0f;
        gVerts.push_back({{-gS, -0.01f, -gS}, gColor});
        gVerts.push_back({{gS, -0.01f, -gS}, gColor});
        gVerts.push_back({{gS, -0.01f, gS}, gColor});
        gVerts.push_back({{-gS, -0.01f, -gS}, gColor});
        gVerts.push_back({{gS, -0.01f, gS}, gColor});
        gVerts.push_back({{-gS, -0.01f, gS}, gColor});
        outerGround = BasicShape(gVerts);

        // 2. السقف العام (على ارتفاع 15 وحدة)
        std::vector<BasicVertex> ceilVerts;
        glm::vec3 ceilColor = {0.15f, 0.15f, 0.18f};
        float hCeil = 15.0f;
        ceilVerts.push_back({{-gS, hCeil, -gS}, ceilColor});
        ceilVerts.push_back({{gS, hCeil, -gS}, ceilColor});
        ceilVerts.push_back({{gS, hCeil, gS}, ceilColor});
        ceilVerts.push_back({{-gS, hCeil, -gS}, ceilColor});
        ceilVerts.push_back({{gS, hCeil, gS}, ceilColor});
        ceilVerts.push_back({{-gS, hCeil, gS}, ceilColor});
        ceiling = BasicShape(ceilVerts);



        // 4. بناء 4 أعمدة في الساحة المركزية
        std::vector<BasicVertex> colVerts;
        glm::vec3 colCol = {0.2f, 0.2f, 0.25f};
        auto addColumn = [&](float x, float z)
        {
            float cw = 1.5f; // عرض العمود
            // رسم جوانب العمود الأربعة من الأرض للسقف
            colVerts.push_back({{x - cw, 0, z + cw}, colCol});
            colVerts.push_back({{x + cw, 0, z + cw}, colCol});
            colVerts.push_back({{x + cw, hCeil, z + cw}, colCol});
            colVerts.push_back({{x - cw, 0, z + cw}, colCol});
            colVerts.push_back({{x + cw, hCeil, z + cw}, colCol});
            colVerts.push_back({{x - cw, hCeil, z + cw}, colCol});
            // (نكتفي بوجه واحد مواجه للمركز للتبسيط حالياً)
        };
        addColumn(15, 15);
        addColumn(-15, 15);
        addColumn(15, -15);
        addColumn(-15, -15);
        columns = BasicShape(colVerts);

        // 5. الغرف (تعديل طفيف لزيادة الحجم)
        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Luxury", {45, 0, -45}, 15.0f, {0.15f, 0.15f, 0.15f}, true);
        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Electric", {-45, 0, -45}, 15.0f, {0.0f, 0.2f, 0.5f}, true);
        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Sports", {45, 0, 45}, 15.0f, {0.6f, 0.0f, 0.0f}, false);
        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Family", {-45, 0, 45}, 15.0f, {0.0f, 0.5f, 0.0f}, false);

        std::vector<BasicVertex> roadV;
        glm::vec3 asphaltCol = { 0.15f, 0.15f, 0.15f }; // لون رمادي غامق (زفت)
        float roadW = 8.0f;  // نصف عرض الطريق
        float roadL = 100.0f; // طول الطريق

        roadV.push_back({ {-roadW, 0.05f, -roadL}, asphaltCol }); roadV.push_back({ {roadW, 0.01f, -roadL}, asphaltCol });
        roadV.push_back({ {roadW, 0.05f,  roadL}, asphaltCol }); roadV.push_back({ {-roadW, 0.01f, -roadL}, asphaltCol });
        roadV.push_back({ {roadW, 0.05f,  roadL}, asphaltCol }); roadV.push_back({ {-roadW, 0.01f,  roadL}, asphaltCol });
        streetAsphalt = BasicShape(roadV);

        std::vector<BasicVertex> sideV;
        glm::vec3 darkGrey = { 0.3f, 0.3f, 0.3f };  // لون الفواصل
        glm::vec3 lightGrey = { 0.7f, 0.7f, 0.7f }; // لون البلاطة
        glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };

        float sW = 1.5f;     // عرض الرصيف
        float sH = 0.4f;     // ارتفاع الرصيف (السماكة)
        float tileL = 2.0f;  // طول البلاطة الواحدة

        auto addDetailedSidewalk = [&](float xPos) {
            for (float z = -roadL; z < roadL; z += tileL) {
                // نغير اللون بين بلاطة والتانية بشيء بسيط عشان يبين التقسيم
                glm::vec3 currentCol = ((int)((z + roadL) / tileL) % 2 == 0) ? lightGrey : lightGrey * 0.9f;

                // إحداثيات البلاطة
                float xMin = xPos - sW;
                float xMax = xPos + sW;
                float zMin = z;
                float zMax = z + tileL - 0.1f; // طرحنا 0.1 عشان نترك فراغ (فاصل) بين البلاطات
                float yBottom = 0.05f;         // مستوى الزفت
                float yTop = 0.45f;            // سطح الرصيف المرفوع

                // 1. السطح العلوي (Top)
                sideV.push_back({ {xMin, yTop, zMin}, currentCol }); sideV.push_back({ {xMax, yTop, zMin}, currentCol });
                sideV.push_back({ {xMax, yTop, zMax}, currentCol }); sideV.push_back({ {xMin, yTop, zMin}, currentCol });
                sideV.push_back({ {xMax, yTop, zMax}, currentCol }); sideV.push_back({ {xMin, yTop, zMax}, currentCol });

                // 2. الجانب المواجه للطريق (Side Face)
                // إذا الرصيف يمين، الوجه الداخلي هو اليسار
                float faceX = (xPos > 0) ? xMin : xMax;
                sideV.push_back({ {faceX, yBottom, zMin}, yellow }); sideV.push_back({ {faceX, yTop, zMin}, darkGrey });
                sideV.push_back({ {faceX, yTop, zMax}, darkGrey });    sideV.push_back({ {faceX, yBottom, zMin}, darkGrey });
                sideV.push_back({ {faceX, yTop, zMax}, darkGrey });    sideV.push_back({ {faceX, yBottom, zMax}, yellow });
            }
            };

        addDetailedSidewalk(roadW + sW);    // رصيف يمين
        addDetailedSidewalk(-(roadW + sW)); // رصيف يسار
        sidewalk = BasicShape(sideV);

        std::vector<BasicVertex> lineV;
        glm::vec3 lineCol = { 1.0f, 1.0f, 1.0f }; // أبيض ناصع
        for (float z = -roadL; z < roadL; z += 10.0f) {
            float lW = 0.15f; // عرض الخط
            float lL = 4.0f;  // طول الخط
            lineV.push_back({ {-lW, 0.15f, z}, lineCol }); lineV.push_back({ {lW, 0.15f, z}, lineCol });
            lineV.push_back({ {lW, 0.15f, z + lL}, lineCol }); lineV.push_back({ {-lW, 0.15f, z}, lineCol });
            lineV.push_back({ {lW, 0.15f, z + lL}, lineCol }); lineV.push_back({ {-lW, 0.15f, z + lL}, lineCol });
        }
        streetLines = BasicShape(lineV);
    }

    // لا تنسى تحديث renderAll لرسم المكتب
    void Showroom::renderAll(const glm::mat4 &viewProj)
    {
        outerGround.render(glm::mat4(1.0f), viewProj);
        ceiling.render(glm::mat4(1.0f), viewProj); // رسم السقف
        columns.render(glm::mat4(1.0f), viewProj); // رسم الأعمدة
        streetAsphalt.render(glm::mat4(1.0f), viewProj); // رسم الزفت
        sidewalk.render(glm::mat4(1.0f), viewProj);      // رسم الرصيف
        streetLines.render(glm::mat4(1.0f), viewProj);

        for (auto &r : rooms)
            r->draw(viewProj);

;
    }

    glm::vec3 Showroom::checkCollision(glm::vec3 currentPos, glm::vec3 nextPos)
    {
        float margin = 0.7f;
        for (auto &room : rooms)
        {
            float xMin = room->centerOffset.x - room->size;
            float xMax = room->centerOffset.x + room->size;
            float zMin = room->centerOffset.z - room->size;
            float zMax = room->centerOffset.z + room->size;
            float dL = room->centerOffset.x - 2.5f;
            float dR = room->centerOffset.x + 2.5f;

            // إذا كان اللاعب داخل أو يلمس حدود الغرفة
            if (nextPos.x > xMin - margin && nextPos.x < xMax + margin &&
                nextPos.z > zMin - margin && nextPos.z < zMax + margin)
            {
                // الجدران الجانبية (دائماً مصمتة)
                if (nextPos.x < xMin + margin)
                    nextPos.x = xMin + margin;
                if (nextPos.x > xMax - margin)
                    nextPos.x = xMax - margin;

                // تحديد أي جدار هو الباب
                if (room->doorAtMaxZ)
                { // الباب عند zMax (الغرف الخلفية)
                    if (nextPos.z < zMin + margin)
                        nextPos.z = zMin + margin; // جدار خلفي مصمت
                    if (nextPos.x < dL || nextPos.x > dR)
                    { // خارج عرض الباب
                        if (currentPos.z < zMax)
                        {
                            if (nextPos.z > zMax - margin)
                                nextPos.z = zMax - margin;
                        }
                        else
                        {
                            if (nextPos.z < zMax + margin)
                                nextPos.z = zMax + margin;
                        }
                    }
                }
                else
                { // الباب عند zMin (الغرف الأمامية)
                    if (nextPos.z > zMax - margin)
                        nextPos.z = zMax - margin; // جدار أمامي مصمت (بالنسبة للغرفة)
                    if (nextPos.x < dL || nextPos.x > dR)
                    { // خارج عرض الباب
                        if (currentPos.z > zMin)
                        {
                            if (nextPos.z < zMin + margin)
                                nextPos.z = zMin + margin;
                        }
                        else
                        {
                            if (nextPos.z > zMin - margin)
                                nextPos.z = zMin - margin;
                        }
                    }
                }
            }
        }
        return nextPos;
    }
}