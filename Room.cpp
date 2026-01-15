#include "Room.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Example
{
    void Room::create(std::string roomName, glm::vec3 position, float roomSize, glm::vec3 color, bool isDoorAtMaxZ)
    {
        this->name = roomName;
        this->centerOffset = position;
        this->size = roomSize;
        this->doorAtMaxZ = isDoorAtMaxZ;

        float h = 7.0f;
        float doorWidth = 5.0f;
        std::vector<BasicVertex> floorV, wallV, lightV;
        auto addPos = [&](glm::vec3 p) { return p + centerOffset; };

        // ═══════════════════════════════════════════════════════════
        // 1. الأرضية
        // ═══════════════════════════════════════════════════════════
        glm::vec3 floorColor = color * 0.2f;
        floorV.push_back({ addPos({-size, 0, -size}), floorColor });
        floorV.push_back({ addPos({size, 0, -size}), floorColor });
        floorV.push_back({ addPos({size, 0, size}), floorColor });
        floorV.push_back({ addPos({-size, 0, -size}), floorColor });
        floorV.push_back({ addPos({size, 0, size}), floorColor });
        floorV.push_back({ addPos({-size, 0, size}), floorColor });
        floor = BasicShape(floorV);

        // ═══════════════════════════════════════════════════════════
        // 2. المنصات الثلاث 🚗🚗🚗
        // ═══════════════════════════════════════════════════════════
        float pW = 3.5f;      // نصف عرض المنصة (X)
        float pD = 4.5f;      // نصف عمق المنصة (Z)
        float pH = 0.25f;     // ارتفاع المنصة
        float spacing = 9.0f; // المسافة بين مراكز المنصات

        // موقع المنصات على محور Z (بعيداً عن الباب)
        float pZ = isDoorAtMaxZ ? -size * 0.35f : size * 0.35f;

        // ألوان المنصة
        glm::vec3 pTopColor = { 0.75f, 0.75f, 0.78f };    // السطح العلوي - رمادي فاتح
        glm::vec3 pSideColor = { 0.45f, 0.45f, 0.50f };   // الجوانب - رمادي متوسط
        glm::vec3 pEdgeColor = { 0.3f, 0.3f, 0.35f };     // الحواف - رمادي غامق

        // مواقع المنصات الثلاث على محور X
        float podiumPositions[3] = { -spacing, 0.0f, spacing };

        podiums.clear();
        podiumBounds.clear();

        for (int i = 0; i < 3; i++)
        {
            float pX = podiumPositions[i];
            std::vector<BasicVertex> podiumV;

            // ─────────────────────────────────────────
            // السطح العلوي (Top Face)
            // ─────────────────────────────────────────
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pTopColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pTopColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pTopColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pTopColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pTopColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pTopColor });

            // ─────────────────────────────────────────
            // الجانب الأمامي (Front Face - Z+)
            // ─────────────────────────────────────────
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor });

            // ─────────────────────────────────────────
            // الجانب الخلفي (Back Face - Z-)
            // ─────────────────────────────────────────
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor });

            // ─────────────────────────────────────────
            // الجانب الأيسر (Left Face - X-)
            // ─────────────────────────────────────────
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor });

            // ─────────────────────────────────────────
            // الجانب الأيمن (Right Face - X+)
            // ─────────────────────────────────────────
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor });

            podiums.push_back(BasicShape(podiumV));

            // ✅ حفظ حدود المنصة للتصادم
            podiumBounds.push_back({
                centerOffset.x + pX,  // xCenter
                centerOffset.z + pZ,  // zCenter
                pW,                   // halfWidth
                pD                    // halfDepth
                });
        }

        // ═══════════════════════════════════════════════════════════
        // 3. إضاءة السقف 💡
        // ═══════════════════════════════════════════════════════════
        glm::vec3 lCol = { 1.0f, 1.0f, 0.95f }; // أبيض دافئ
        float lS = size * 0.35f;
        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({lS, h - 0.01f, lS}), lCol });
        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({lS, h - 0.01f, lS}), lCol });
        lightV.push_back({ addPos({-lS, h - 0.01f, lS}), lCol });
        ceilingLight = BasicShape(lightV);

        // ═══════════════════════════════════════════════════════════
        // 4. الجدران 🧱
        // ═══════════════════════════════════════════════════════════
        auto addWall = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
            {
                wallV.push_back({ addPos(p1), color });
                wallV.push_back({ addPos(p2), color });
                wallV.push_back({ addPos(p3), color });
                wallV.push_back({ addPos(p1), color });
                wallV.push_back({ addPos(p3), color });
                wallV.push_back({ addPos(p4), color });
            };

        // الجدار الأيسر
        addWall({ -size, 0, -size }, { -size, 0, size }, { -size, h, size }, { -size, h, -size });
        // الجدار الأيمن
        addWall({ size, 0, -size }, { size, 0, size }, { size, h, size }, { size, h, -size });

        // الجدار المصمت (بدون باب)
        float doorZ = isDoorAtMaxZ ? size : -size;
        float solidZ = isDoorAtMaxZ ? -size : size;
        addWall({ -size, 0, solidZ }, { size, 0, solidZ }, { size, h, solidZ }, { -size, h, solidZ });

        // جدار الباب (3 أجزاء)
        addWall({ -size, 0, doorZ }, { -doorWidth / 2, 0, doorZ }, { -doorWidth / 2, h, doorZ }, { -size, h, doorZ });
        addWall({ doorWidth / 2, 0, doorZ }, { size, 0, doorZ }, { size, h, doorZ }, { doorWidth / 2, h, doorZ });
        addWall({ -doorWidth / 2, 4.5f, doorZ }, { doorWidth / 2, 4.5f, doorZ }, { doorWidth / 2, h, doorZ }, { -doorWidth / 2, h, doorZ });

        walls = BasicShape(wallV);
    }

    void Room::draw(const glm::mat4& viewProj)
    {
        floor.render(glm::mat4(1.0f), viewProj);
        walls.render(glm::mat4(1.0f), viewProj);

        // ✅ رسم المنصات الثلاث
        for (auto& podium : podiums)
        {
            podium.render(glm::mat4(1.0f), viewProj);
        }

        ceilingLight.render(glm::mat4(1.0f), viewProj);
    }
}