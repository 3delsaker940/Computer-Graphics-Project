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

        // 3. مكتب الاستقبال (تحسين الشكل)
        std::vector<BasicVertex> deskV;
        glm::vec3 bodyCol = {0.4f, 0.4f, 0.45f}; // لون جسم المكتب
        glm::vec3 topCol = {0.8f, 0.8f, 0.8f};   // لون السطح
        float dW = 5.0f, dH = 1.3f, dD = 2.0f;

        auto addCube = [&](glm::vec3 p, float w, float h, float d, glm::vec3 c)
        {
            // وجه أمامي
            deskV.push_back({{p.x - w, p.y, p.z + d}, c});
            deskV.push_back({{p.x + w, p.y, p.z + d}, c});
            deskV.push_back({{p.x + w, p.y + h, p.z + d}, c});
            deskV.push_back({{p.x - w, p.y, p.z + d}, c});
            deskV.push_back({{p.x + w, p.y + h, p.z + d}, c});
            deskV.push_back({{p.x - w, p.y + h, p.z + d}, c});
            // سطح المكتب
            deskV.push_back({{p.x - w, p.y + h, p.z - d}, topCol});
            deskV.push_back({{p.x + w, p.y + h, p.z - d}, topCol});
            deskV.push_back({{p.x + w, p.y + h, p.z + d}, topCol});
            deskV.push_back({{p.x - w, p.y + h, p.z - d}, topCol});
            deskV.push_back({{p.x + w, p.y + h, p.z + d}, topCol});
            deskV.push_back({{p.x - w, p.y + h, p.z + d}, topCol});
        };
        addCube({0, 0, 0}, dW, dH, dD, bodyCol);
        receptionDesk = BasicShape(deskV);

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
    }

    // لا تنسى تحديث renderAll لرسم المكتب
    void Showroom::renderAll(const glm::mat4 &viewProj)
    {
        outerGround.render(glm::mat4(1.0f), viewProj);
        ceiling.render(glm::mat4(1.0f), viewProj); // رسم السقف
        columns.render(glm::mat4(1.0f), viewProj); // رسم الأعمدة
        receptionDesk.render(glm::mat4(1.0f), viewProj);

        for (auto &r : rooms)
            r->draw(viewProj);
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