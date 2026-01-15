#include "Showroom.hpp"
#include <iostream>
#include <cmath>

namespace Example
{
    void Showroom::init()
    {
        // ═══════════════════════════════════════════════════════════
        // ثوابت المعرض
        // ═══════════════════════════════════════════════════════════
        float showroomSize = 70.0f;    // نصف حجم المعرض
        float wallHeight = 15.0f;      // ارتفاع الجدران
        float wallThickness = 0.5f;    // سماكة الجدار

        // الباب الرئيسي
        float mainDoorWidth = 10.0f;   // عرض الباب
        float mainDoorHeight = 5.0f;   // ارتفاع الباب
        float mainDoorHalfW = mainDoorWidth / 2.0f;

        // النوافذ
        float windowWidth = 35.0f;     // عرض النافذة
        float windowBottom = 1.5f;     // ارتفاع بداية النافذة
        float windowTop = 12.0f;       // ارتفاع نهاية النافذة
        float windowHalfW = windowWidth / 2.0f;

        // ✅ حفظ حدود التصادم
        exteriorBounds.minX = -showroomSize;
        exteriorBounds.maxX = showroomSize;
        exteriorBounds.minZ = -showroomSize;
        exteriorBounds.maxZ = showroomSize;
        exteriorBounds.doorMinX = -mainDoorHalfW;
        exteriorBounds.doorMaxX = mainDoorHalfW;
        exteriorBounds.doorZ = showroomSize;

        // ═══════════════════════════════════════════════════════════
        // 1. الأرضية الكبرى
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> gVerts;
        glm::vec3 gColor = { 0.08f, 0.08f, 0.10f };
        float gS = 150.0f;
        gVerts.push_back({ {-gS, -0.01f, -gS}, gColor });
        gVerts.push_back({ {gS, -0.01f, -gS}, gColor });
        gVerts.push_back({ {gS, -0.01f, gS}, gColor });
        gVerts.push_back({ {-gS, -0.01f, -gS}, gColor });
        gVerts.push_back({ {gS, -0.01f, gS}, gColor });
        gVerts.push_back({ {-gS, -0.01f, gS}, gColor });
        outerGround = BasicShape(gVerts);

        // ═══════════════════════════════════════════════════════════
        // 2. السقف
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> ceilVerts;
        glm::vec3 ceilColor = { 0.12f, 0.12f, 0.15f };
        ceilVerts.push_back({ {-showroomSize, wallHeight, -showroomSize}, ceilColor });
        ceilVerts.push_back({ {showroomSize, wallHeight, -showroomSize}, ceilColor });
        ceilVerts.push_back({ {showroomSize, wallHeight, showroomSize}, ceilColor });
        ceilVerts.push_back({ {-showroomSize, wallHeight, -showroomSize}, ceilColor });
        ceilVerts.push_back({ {showroomSize, wallHeight, showroomSize}, ceilColor });
        ceilVerts.push_back({ {-showroomSize, wallHeight, showroomSize}, ceilColor });
        ceiling = BasicShape(ceilVerts);

        // ═══════════════════════════════════════════════════════════
        // 3. الجدران الخارجية ✅
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> wallVerts;
        glm::vec3 wallColorOut = { 0.25f, 0.25f, 0.28f };   // اللون الخارجي
        glm::vec3 wallColorIn = { 0.35f, 0.35f, 0.38f };    // اللون الداخلي
        glm::vec3 wallColorDark = { 0.18f, 0.18f, 0.20f };  // الحواف

        float S = showroomSize;
        float H = wallHeight;

        // دالة مساعدة لإضافة مستطيل (وجه جدار)
        auto addQuad = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 col)
            {
                wallVerts.push_back({ p1, col });
                wallVerts.push_back({ p2, col });
                wallVerts.push_back({ p3, col });
                wallVerts.push_back({ p1, col });
                wallVerts.push_back({ p3, col });
                wallVerts.push_back({ p4, col });
            };

        // ─────────────────────────────────────────
        // الجدار الخلفي (Z = -S) - مع نافذة
        // ─────────────────────────────────────────
        // الجزء الأيسر من النافذة
        addQuad({ -S, 0, -S }, { -windowHalfW, 0, -S }, { -windowHalfW, H, -S }, { -S, H, -S }, wallColorIn);
        // الجزء الأيمن من النافذة
        addQuad({ windowHalfW, 0, -S }, { S, 0, -S }, { S, H, -S }, { windowHalfW, H, -S }, wallColorIn);
        // الجزء تحت النافذة
        addQuad({ -windowHalfW, 0, -S }, { windowHalfW, 0, -S }, { windowHalfW, windowBottom, -S }, { -windowHalfW, windowBottom, -S }, wallColorIn);
        // الجزء فوق النافذة
        addQuad({ -windowHalfW, windowTop, -S }, { windowHalfW, windowTop, -S }, { windowHalfW, H, -S }, { -windowHalfW, H, -S }, wallColorIn);

        // ─────────────────────────────────────────
        // الجدار الأمامي (Z = +S) - مع الباب الرئيسي
        // ─────────────────────────────────────────
        // الجزء الأيسر من الباب
        addQuad({ -S, 0, S }, { -mainDoorHalfW, 0, S }, { -mainDoorHalfW, H, S }, { -S, H, S }, wallColorOut);
        // الجزء الأيمن من الباب
        addQuad({ mainDoorHalfW, 0, S }, { S, 0, S }, { S, H, S }, { mainDoorHalfW, H, S }, wallColorOut);
        // الجزء فوق الباب
        addQuad({ -mainDoorHalfW, mainDoorHeight, S }, { mainDoorHalfW, mainDoorHeight, S }, { mainDoorHalfW, H, S }, { -mainDoorHalfW, H, S }, wallColorOut);

        // ─────────────────────────────────────────
        // الجدار الأيسر (X = -S) - مع نافذة
        // ─────────────────────────────────────────
        // الجزء الخلفي من النافذة
        addQuad({ -S, 0, -S }, { -S, 0, -windowHalfW }, { -S, H, -windowHalfW }, { -S, H, -S }, wallColorOut);
        // الجزء الأمامي من النافذة
        addQuad({ -S, 0, windowHalfW }, { -S, 0, S }, { -S, H, S }, { -S, H, windowHalfW }, wallColorOut);
        // الجزء تحت النافذة
        addQuad({ -S, 0, -windowHalfW }, { -S, 0, windowHalfW }, { -S, windowBottom, windowHalfW }, { -S, windowBottom, -windowHalfW }, wallColorOut);
        // الجزء فوق النافذة
        addQuad({ -S, windowTop, -windowHalfW }, { -S, windowTop, windowHalfW }, { -S, H, windowHalfW }, { -S, H, -windowHalfW }, wallColorOut);

        // ─────────────────────────────────────────
        // الجدار الأيمن (X = +S) - مع نافذة
        // ─────────────────────────────────────────
        // الجزء الخلفي من النافذة
        addQuad({ S, 0, -windowHalfW }, { S, 0, -S }, { S, H, -S }, { S, H, -windowHalfW }, wallColorOut);
        // الجزء الأمامي من النافذة
        addQuad({ S, 0, S }, { S, 0, windowHalfW }, { S, H, windowHalfW }, { S, H, S }, wallColorOut);
        // الجزء تحت النافذة
        addQuad({ S, 0, windowHalfW }, { S, 0, -windowHalfW }, { S, windowBottom, -windowHalfW }, { S, windowBottom, windowHalfW }, wallColorOut);
        // الجزء فوق النافذة
        addQuad({ S, windowTop, windowHalfW }, { S, windowTop, -windowHalfW }, { S, H, -windowHalfW }, { S, H, windowHalfW }, wallColorOut);

        exteriorWalls = BasicShape(wallVerts);

        // ═══════════════════════════════════════════════════════════
        // 4. زجاج النوافذ (شفاف) ✅
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> glassVerts;
        glm::vec3 glassColor = { 0.6f, 0.8f, 0.9f };  // أزرق سماوي فاتح

        // ✅ إنشاء lambda جديدة باسم مختلف
        auto addGlassQuad = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 col)
            {
                glassVerts.push_back({ p1, col });
                glassVerts.push_back({ p2, col });
                glassVerts.push_back({ p3, col });
                glassVerts.push_back({ p1, col });
                glassVerts.push_back({ p3, col });
                glassVerts.push_back({ p4, col });
            };

        // نافذة خلفية (Z = -S)
        addGlassQuad({ -windowHalfW, windowBottom, -S + 0.05f }, { windowHalfW, windowBottom, -S + 0.05f },
            { windowHalfW, windowTop, -S + 0.05f }, { -windowHalfW, windowTop, -S + 0.05f }, glassColor);

        // نافذة يسار (X = -S)
        addGlassQuad({ -S + 0.05f, windowBottom, -windowHalfW }, { -S + 0.05f, windowBottom, windowHalfW },
            { -S + 0.05f, windowTop, windowHalfW }, { -S + 0.05f, windowTop, -windowHalfW }, glassColor);

        // نافذة يمين (X = +S)
        addGlassQuad({ S - 0.05f, windowBottom, windowHalfW }, { S - 0.05f, windowBottom, -windowHalfW },
            { S - 0.05f, windowTop, -windowHalfW }, { S - 0.05f, windowTop, windowHalfW }, glassColor);

        windowGlass = BasicShape(glassVerts);

        // ═══════════════════════════════════════════════════════════
        // 5. إطارات النوافذ والباب ✅
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> frameVerts;
        glm::vec3 frameColor = { 0.1f, 0.1f, 0.12f };  // رمادي غامق
        float frameW = 0.3f;  // عرض الإطار

        auto addFrame = [&](float x1, float z1, float x2, float z2, float y1, float y2, bool isVertical)
            {
                if (isVertical)
                {
                    // إطار عمودي
                    frameVerts.push_back({ {x1 - frameW, y1, z1}, frameColor });
                    frameVerts.push_back({ {x1 + frameW, y1, z1}, frameColor });
                    frameVerts.push_back({ {x1 + frameW, y2, z1}, frameColor });
                    frameVerts.push_back({ {x1 - frameW, y1, z1}, frameColor });
                    frameVerts.push_back({ {x1 + frameW, y2, z1}, frameColor });
                    frameVerts.push_back({ {x1 - frameW, y2, z1}, frameColor });
                }
                else
                {
                    // إطار أفقي
                    frameVerts.push_back({ {x1, y1 - frameW, z1}, frameColor });
                    frameVerts.push_back({ {x2, y1 - frameW, z2}, frameColor });
                    frameVerts.push_back({ {x2, y1 + frameW, z2}, frameColor });
                    frameVerts.push_back({ {x1, y1 - frameW, z1}, frameColor });
                    frameVerts.push_back({ {x2, y1 + frameW, z2}, frameColor });
                    frameVerts.push_back({ {x1, y1 + frameW, z1}, frameColor });
                }
            };

        // إطارات النافذة الخلفية (Z = -S)
        // إطار يسار
        frameVerts.push_back({ {-windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW + frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW - frameW, windowTop, -S + 0.1f}, frameColor });

        // إطار يمين
        frameVerts.push_back({ {windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW + frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW - frameW, windowTop, -S + 0.1f}, frameColor });

        // إطار علوي
        frameVerts.push_back({ {-windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });

        // إطار سفلي
        frameVerts.push_back({ {-windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });
        frameVerts.push_back({ {-windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });

        windowFrames = BasicShape(frameVerts);

        // ═══════════════════════════════════════════════════════════
        // 6. إطار الباب الرئيسي ✅
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> doorFrameVerts;
        glm::vec3 doorFrameCol = { 0.08f, 0.08f, 0.1f };
        float dfW = 0.4f;  // عرض إطار الباب

        // الإطار الأيسر
        doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });

        // الإطار الأيمن
        doorFrameVerts.push_back({ {mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW + dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW - dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });

        // الإطار العلوي
        doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });
        doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });

        mainDoorFrame = BasicShape(doorFrameVerts);

        // ═══════════════════════════════════════════════════════════
        // 7. الأعمدة المحسّنة
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> colVerts;
        glm::vec3 colMain = { 0.35f, 0.35f, 0.40f };
        glm::vec3 colDark = { 0.25f, 0.25f, 0.30f };
        glm::vec3 colBase = { 0.45f, 0.45f, 0.50f };
        glm::vec3 colAccent = { 0.55f, 0.55f, 0.60f };

        float cw = 1.2f;
        float baseW = 1.6f;
        float baseH = 0.5f;
        float capH = 0.4f;

        float columnPositions[4][2] = {
            {15.0f, 15.0f}, {-15.0f, 15.0f},
            {15.0f, -15.0f}, {-15.0f, -15.0f}
        };

        columnBounds.clear();

        for (int i = 0; i < 4; i++)
        {
            float x = columnPositions[i][0];
            float z = columnPositions[i][1];

            // القاعدة - 4 جوانب + سطح علوي
            // الأمام
            colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });

            // الخلف
            colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });

            // اليسار
            colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });

            // اليمين
            colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
            colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
            colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });

            // السطح العلوي للقاعدة
            colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });

            // الجسم الرئيسي - 4 جوانب
            float bodyBottom = baseH;
            float bodyTop = wallHeight - capH;

            colVerts.push_back({ {x - cw, bodyBottom, z + cw}, colMain });
            colVerts.push_back({ {x + cw, bodyBottom, z + cw}, colMain });
            colVerts.push_back({ {x + cw, bodyTop, z + cw}, colMain });
            colVerts.push_back({ {x - cw, bodyBottom, z + cw}, colMain });
            colVerts.push_back({ {x + cw, bodyTop, z + cw}, colMain });
            colVerts.push_back({ {x - cw, bodyTop, z + cw}, colMain });

            colVerts.push_back({ {x + cw, bodyBottom, z - cw}, colDark });
            colVerts.push_back({ {x - cw, bodyBottom, z - cw}, colDark });
            colVerts.push_back({ {x - cw, bodyTop, z - cw}, colDark });
            colVerts.push_back({ {x + cw, bodyBottom, z - cw}, colDark });
            colVerts.push_back({ {x - cw, bodyTop, z - cw}, colDark });
            colVerts.push_back({ {x + cw, bodyTop, z - cw}, colDark });

            colVerts.push_back({ {x - cw, bodyBottom, z - cw}, colDark });
            colVerts.push_back({ {x - cw, bodyBottom, z + cw}, colDark });
            colVerts.push_back({ {x - cw, bodyTop, z + cw}, colDark });
            colVerts.push_back({ {x - cw, bodyBottom, z - cw}, colDark });
            colVerts.push_back({ {x - cw, bodyTop, z + cw}, colDark });
            colVerts.push_back({ {x - cw, bodyTop, z - cw}, colDark });

            colVerts.push_back({ {x + cw, bodyBottom, z + cw}, colMain });
            colVerts.push_back({ {x + cw, bodyBottom, z - cw}, colMain });
            colVerts.push_back({ {x + cw, bodyTop, z - cw}, colMain });
            colVerts.push_back({ {x + cw, bodyBottom, z + cw}, colMain });
            colVerts.push_back({ {x + cw, bodyTop, z - cw}, colMain });
            colVerts.push_back({ {x + cw, bodyTop, z + cw}, colMain });

            // التاج - 4 جوانب
            float capBottom = wallHeight - capH;
            float capTop = wallHeight;

            colVerts.push_back({ {x - baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capTop, z + baseW}, colBase });
            colVerts.push_back({ {x - baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capTop, z + baseW}, colBase });
            colVerts.push_back({ {x - baseW, capTop, z + baseW}, colBase });

            colVerts.push_back({ {x + baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capTop, z - baseW}, colBase });
            colVerts.push_back({ {x + baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capTop, z - baseW}, colBase });
            colVerts.push_back({ {x + baseW, capTop, z - baseW}, colBase });

            colVerts.push_back({ {x - baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capTop, z + baseW}, colBase });
            colVerts.push_back({ {x - baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x - baseW, capTop, z + baseW}, colBase });
            colVerts.push_back({ {x - baseW, capTop, z - baseW}, colBase });

            colVerts.push_back({ {x + baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capBottom, z - baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capTop, z - baseW}, colBase });
            colVerts.push_back({ {x + baseW, capBottom, z + baseW}, colAccent });
            colVerts.push_back({ {x + baseW, capTop, z - baseW}, colBase });
            colVerts.push_back({ {x + baseW, capTop, z + baseW}, colBase });

            columnBounds.push_back({ x, z, baseW });
        }

        columns = BasicShape(colVerts);

        // ═══════════════════════════════════════════════════════════
        // 8. الغرف الأربع
        // ═══════════════════════════════════════════════════════════
        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Luxury", { 45, 0, -45 }, 15.0f, { 0.15f, 0.15f, 0.15f }, true);

        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Electric", { -45, 0, -45 }, 15.0f, { 0.0f, 0.2f, 0.5f }, true);

        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Sports", { 45, 0, 45 }, 15.0f, { 0.6f, 0.0f, 0.0f }, false);

        rooms.push_back(std::make_unique<Room>());
        rooms.back()->create("Family", { -45, 0, 45 }, 15.0f, { 0.0f, 0.5f, 0.0f }, false);


        // ═══════════════════════════════════════════════════════════
// 10. إضافة السيارات ✅
// ═══════════════════════════════════════════════════════════

// سيارة في غرفة Luxury (المنصة الوسطى - index 1) - لون أحمر
        rooms[0]->addCarToPodium(1, { 0.7f, 0.1f, 0.1f }, { 0.2f, 0.15f, 0.1f });

        // سيارة في غرفة Electric (المنصة الوسطى) - لون أزرق
        rooms[1]->addCarToPodium(1, { 0.1f, 0.3f, 0.7f }, { 0.1f, 0.1f, 0.15f });

        // سيارة في غرفة Sports (المنصة الوسطى) - لون أصفر
        rooms[2]->addCarToPodium(1, { 0.9f, 0.7f, 0.1f }, { 0.1f, 0.08f, 0.05f });

        // سيارة في غرفة Family (المنصة الوسطى) - لون أبيض
        rooms[3]->addCarToPodium(1, { 0.9f, 0.9f, 0.92f }, { 0.12f, 0.1f, 0.08f });

        std::cout << "✅ Cars added to showroom!" << std::endl;
        std::cout << "💡 Press E near a car to enter/exit" << std::endl;

        // ═══════════════════════════════════════════════════════════
        // 9. الشارع والرصيف
        // ═══════════════════════════════════════════════════════════
        std::vector<BasicVertex> roadV;
        glm::vec3 asphaltCol = { 0.15f, 0.15f, 0.15f };
        float roadW = 8.0f;
        float roadL = 150.0f;

        roadV.push_back({ {-roadW, 0.02f, -roadL}, asphaltCol });
        roadV.push_back({ {roadW, 0.02f, -roadL}, asphaltCol });
        roadV.push_back({ {roadW, 0.02f, roadL}, asphaltCol });
        roadV.push_back({ {-roadW, 0.02f, -roadL}, asphaltCol });
        roadV.push_back({ {roadW, 0.02f, roadL}, asphaltCol });
        roadV.push_back({ {-roadW, 0.02f, roadL}, asphaltCol });
        streetAsphalt = BasicShape(roadV);

        std::vector<BasicVertex> sideV;
        glm::vec3 lightGrey = { 0.7f, 0.7f, 0.7f };
        glm::vec3 darkGrey = { 0.3f, 0.3f, 0.3f };
        glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };
        float sW = 1.5f;
        float tileL = 2.0f;

        auto addDetailedSidewalk = [&](float xPos)
            {
                for (float z = -roadL; z < roadL; z += tileL)
                {
                    glm::vec3 currentCol = ((int)((z + roadL) / tileL) % 2 == 0) ? lightGrey : lightGrey * 0.9f;
                    float xMin = xPos - sW;
                    float xMax = xPos + sW;
                    float zMin = z;
                    float zMax = z + tileL - 0.1f;
                    float yTop = 0.45f;
                    float yBottom = 0.02f;

                    sideV.push_back({ {xMin, yTop, zMin}, currentCol });
                    sideV.push_back({ {xMax, yTop, zMin}, currentCol });
                    sideV.push_back({ {xMax, yTop, zMax}, currentCol });
                    sideV.push_back({ {xMin, yTop, zMin}, currentCol });
                    sideV.push_back({ {xMax, yTop, zMax}, currentCol });
                    sideV.push_back({ {xMin, yTop, zMax}, currentCol });

                    float faceX = (xPos > 0) ? xMin : xMax;
                    sideV.push_back({ {faceX, yBottom, zMin}, yellow });
                    sideV.push_back({ {faceX, yTop, zMin}, darkGrey });
                    sideV.push_back({ {faceX, yTop, zMax}, darkGrey });
                    sideV.push_back({ {faceX, yBottom, zMin}, darkGrey });
                    sideV.push_back({ {faceX, yTop, zMax}, darkGrey });
                    sideV.push_back({ {faceX, yBottom, zMax}, yellow });
                }
            };

        addDetailedSidewalk(roadW + sW);
        addDetailedSidewalk(-(roadW + sW));
        sidewalk = BasicShape(sideV);

        std::vector<BasicVertex> lineV;
        glm::vec3 lineCol = { 1.0f, 1.0f, 1.0f };
        for (float z = -roadL; z < roadL; z += 10.0f)
        {
            float lW = 0.15f;
            float lL = 4.0f;
            lineV.push_back({ {-lW, 0.08f, z}, lineCol });
            lineV.push_back({ {lW, 0.08f, z}, lineCol });
            lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
            lineV.push_back({ {-lW, 0.08f, z}, lineCol });
            lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
            lineV.push_back({ {-lW, 0.08f, z + lL}, lineCol });
        }
        streetLines = BasicShape(lineV);

        std::cout << "✅ Showroom initialized with exterior walls and windows!" << std::endl;

        // ═══════════════════════════════════════════════════════════
// 11. إعداد الإضاءة ✅
// ═══════════════════════════════════════════════════════════

// الإضاءة المحيطة
        lighting.ambientColor = { 0.1f, 0.1f, 0.15f };
        lighting.ambientIntensity = 0.25f;

        // الإضاءة الاتجاهية (ضوء قادم من النوافذ)
        lighting.directionalLight = DirectionalLight(
            { 0.2f, -0.8f, 0.3f },      // الاتجاه
            { 1.0f, 0.95f, 0.85f },     // لون دافئ
            0.3f                       // شدة معتدلة
        );

        // ─────────────────────────────────────────────────────────
        // مصابيح السقف الرئيسية (Point Lights)
        // ─────────────────────────────────────────────────────────

        // مصابيح في وسط المعرض
        lighting.addPointLight(PointLight({ 0, 14, 0 }, { 1.0f, 1.0f, 0.95f }, 1.5f, 40.0f));
        lighting.addPointLight(PointLight({ 25, 14, 25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
        lighting.addPointLight(PointLight({ -25, 14, 25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
        lighting.addPointLight(PointLight({ 25, 14, -25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
        lighting.addPointLight(PointLight({ -25, 14, -25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));

        // ─────────────────────────────────────────────────────────
        // مصابيح الغرف (ألوان مختلفة لكل غرفة)
        // ─────────────────────────────────────────────────────────

        // Luxury Room (45, -45) - إضاءة ذهبية دافئة
        lighting.addPointLight(PointLight({ 45, 6.5f, -45 }, { 1.0f, 0.9f, 0.7f }, 1.2f, 25.0f));

        // Electric Room (-45, -45) - إضاءة زرقاء
        lighting.addPointLight(PointLight({ -45, 6.5f, -45 }, { 0.5f, 0.7f, 1.0f }, 1.2f, 25.0f));

        // Sports Room (45, 45) - إضاءة حمراء
        lighting.addPointLight(PointLight({ 45, 6.5f, 45 }, { 1.0f, 0.6f, 0.5f }, 1.2f, 25.0f));

        // Family Room (-45, 45) - إضاءة خضراء طبيعية
        lighting.addPointLight(PointLight({ -45, 6.5f, 45 }, { 0.8f, 1.0f, 0.7f }, 1.2f, 25.0f));

        // ─────────────────────────────────────────────────────────
        // Spot Lights على السيارات
        // ─────────────────────────────────────────────────────────

        // Luxury Car
        lighting.addSpotLight(SpotLight(
            { 45, 6.0f, -50.25f },      // موقع فوق السيارة
            { 0, -1, 0 },               // يشير للأسفل
            { 1.0f, 0.95f, 0.8f },      // لون دافئ
            2.0f,                      // شدة
            30.0f, 45.0f              // زوايا القطع
        ));

        // Electric Car
        lighting.addSpotLight(SpotLight(
            { -45, 6.0f, -50.25f },
            { 0, -1, 0 },
            { 0.6f, 0.8f, 1.0f },       // لون أزرق
            2.0f,
            30.0f, 45.0f
        ));

        // Sports Car
        lighting.addSpotLight(SpotLight(
            { 45, 6.0f, 39.75f },
            { 0, -1, 0 },
            { 1.0f, 0.9f, 0.8f },
            2.0f,
            30.0f, 45.0f
        ));

        // Family Car
        lighting.addSpotLight(SpotLight(
            { -45, 6.0f, 39.75f },
            { 0, -1, 0 },
            { 0.9f, 1.0f, 0.85f },      // لون طبيعي
            2.0f,
            30.0f, 45.0f
        ));

        std::cout << "✅ Lighting system initialized!" << std::endl;
        std::cout << "   Point Lights: " << lighting.pointLights.size() << std::endl;
        std::cout << "   Spot Lights: " << lighting.spotLights.size() << std::endl;
        std::cout << "   Press L to toggle lights" << std::endl;
    }

    void Showroom::renderAll(const glm::mat4& viewProj)
    {
        // ✅ تطبيق الإضاءة على الشيدر
        lighting.applyToShader(BasicShape::getShaderProgram());

        // رسم العناصر الصلبة
        outerGround.render(glm::mat4(1.0f), viewProj);
        ceiling.render(glm::mat4(1.0f), viewProj);
        exteriorWalls.render(glm::mat4(1.0f), viewProj);
        windowFrames.render(glm::mat4(1.0f), viewProj);
        mainDoorFrame.render(glm::mat4(1.0f), viewProj);
        columns.render(glm::mat4(1.0f), viewProj);
        streetAsphalt.render(glm::mat4(1.0f), viewProj);
        sidewalk.render(glm::mat4(1.0f), viewProj);
        streetLines.render(glm::mat4(1.0f), viewProj);

        for (auto& r : rooms)
            r->draw(viewProj);

        // الزجاج الشفاف أخيراً
        windowGlass.render(glm::mat4(1.0f), viewProj, 0.3f);
    }

    void Showroom::toggleLights()
    {
        lighting.toggleAllLights();
        std::cout << "💡 Lights toggled!" << std::endl;
    }

    // ═══════════════════════════════════════════════════════════════════
    // نظام التصادم الشامل
    // ═══════════════════════════════════════════════════════════════════
    glm::vec3 Showroom::checkCollision(glm::vec3 currentPos, glm::vec3 nextPos)
    {
        float margin = 0.5f;
        float podiumMargin = 0.4f;
        float columnMargin = 0.3f;

        // ─────────────────────────────────────────────────────────────
        // 1. ✅ التصادم مع الجدران الخارجية
        // ─────────────────────────────────────────────────────────────
        float extMinX = exteriorBounds.minX;
        float extMaxX = exteriorBounds.maxX;
        float extMinZ = exteriorBounds.minZ;
        float extMaxZ = exteriorBounds.maxZ;
        float doorLeft = exteriorBounds.doorMinX;
        float doorRight = exteriorBounds.doorMaxX;
        float doorZ = exteriorBounds.doorZ;

        // الجدار الأيسر (X = extMinX)
        if (nextPos.z > extMinZ && nextPos.z < extMaxZ)
        {
            if (nextPos.x < extMinX + margin)
                nextPos.x = extMinX + margin;
        }

        // الجدار الأيمن (X = extMaxX)
        if (nextPos.z > extMinZ && nextPos.z < extMaxZ)
        {
            if (nextPos.x > extMaxX - margin)
                nextPos.x = extMaxX - margin;
        }

        // الجدار الخلفي (Z = extMinZ) - بدون باب
        if (nextPos.x > extMinX && nextPos.x < extMaxX)
        {
            if (nextPos.z < extMinZ + margin)
                nextPos.z = extMinZ + margin;
        }

        // الجدار الأمامي (Z = extMaxZ) - مع الباب الرئيسي
        if (nextPos.x > extMinX && nextPos.x < extMaxX)
        {
            bool outsideDoor = (nextPos.x < doorLeft || nextPos.x > doorRight);
            if (outsideDoor)
            {
                // من الداخل للخارج
                if (currentPos.z < doorZ && nextPos.z > doorZ - margin)
                    nextPos.z = doorZ - margin;
                // من الخارج للداخل
                else if (currentPos.z > doorZ && nextPos.z < doorZ + margin)
                    nextPos.z = doorZ + margin;
            }
        }

        // ─────────────────────────────────────────────────────────────
        // 2. التصادم مع الأعمدة
        // ─────────────────────────────────────────────────────────────
        for (const auto& col : columnBounds)
        {
            float colXMin = col.xCenter - col.halfSize;
            float colXMax = col.xCenter + col.halfSize;
            float colZMin = col.zCenter - col.halfSize;
            float colZMax = col.zCenter + col.halfSize;

            bool insideX = nextPos.x > colXMin - columnMargin && nextPos.x < colXMax + columnMargin;
            bool insideZ = nextPos.z > colZMin - columnMargin && nextPos.z < colZMax + columnMargin;

            if (insideX && insideZ)
            {
                float distLeft = std::abs(nextPos.x - (colXMin - columnMargin));
                float distRight = std::abs(nextPos.x - (colXMax + columnMargin));
                float distFront = std::abs(nextPos.z - (colZMax + columnMargin));
                float distBack = std::abs(nextPos.z - (colZMin - columnMargin));

                float minDist = std::min({ distLeft, distRight, distFront, distBack });

                if (minDist == distLeft)
                    nextPos.x = colXMin - columnMargin;
                else if (minDist == distRight)
                    nextPos.x = colXMax + columnMargin;
                else if (minDist == distFront)
                    nextPos.z = colZMax + columnMargin;
                else if (minDist == distBack)
                    nextPos.z = colZMin - columnMargin;
            }
        }

        // ─────────────────────────────────────────────────────────────
        // 3. التصادم مع جدران الغرف والمنصات
        // ─────────────────────────────────────────────────────────────
        for (auto& room : rooms)
        {
            float xMin = room->centerOffset.x - room->size;
            float xMax = room->centerOffset.x + room->size;
            float zMin = room->centerOffset.z - room->size;
            float zMax = room->centerOffset.z + room->size;

            float doorHalfWidth = 2.5f;
            float dL = room->centerOffset.x - doorHalfWidth;
            float dR = room->centerOffset.x + doorHalfWidth;

            float roomDoorZ = room->doorAtMaxZ ? zMax : zMin;
            float solidZ = room->doorAtMaxZ ? zMin : zMax;

            // الجدار الأيسر
            if (nextPos.z > zMin && nextPos.z < zMax)
            {
                if (currentPos.x <= xMin && nextPos.x > xMin - margin)
                    nextPos.x = xMin - margin;
                else if (currentPos.x >= xMin && nextPos.x < xMin + margin)
                    nextPos.x = xMin + margin;
            }

            // الجدار الأيمن
            if (nextPos.z > zMin && nextPos.z < zMax)
            {
                if (currentPos.x >= xMax && nextPos.x < xMax + margin)
                    nextPos.x = xMax + margin;
                else if (currentPos.x <= xMax && nextPos.x > xMax - margin)
                    nextPos.x = xMax - margin;
            }

            // الجدار المصمت
            if (nextPos.x > xMin && nextPos.x < xMax)
            {
                if (room->doorAtMaxZ)
                {
                    if (currentPos.z <= zMin && nextPos.z > zMin - margin)
                        nextPos.z = zMin - margin;
                    else if (currentPos.z >= zMin && nextPos.z < zMin + margin)
                        nextPos.z = zMin + margin;
                }
                else
                {
                    if (currentPos.z >= zMax && nextPos.z < zMax + margin)
                        nextPos.z = zMax + margin;
                    else if (currentPos.z <= zMax && nextPos.z > zMax - margin)
                        nextPos.z = zMax - margin;
                }
            }

            // جدار الباب
            if (nextPos.x > xMin && nextPos.x < xMax)
            {
                bool outsideDoorWidth = (nextPos.x < dL || nextPos.x > dR);
                if (outsideDoorWidth)
                {
                    if (room->doorAtMaxZ)
                    {
                        if (currentPos.z >= zMax && nextPos.z < zMax + margin)
                            nextPos.z = zMax + margin;
                        else if (currentPos.z <= zMax && nextPos.z > zMax - margin)
                            nextPos.z = zMax - margin;
                    }
                    else
                    {
                        if (currentPos.z <= zMin && nextPos.z > zMin - margin)
                            nextPos.z = zMin - margin;
                        else if (currentPos.z >= zMin && nextPos.z < zMin + margin)
                            nextPos.z = zMin + margin;
                    }
                }
            }

            // التصادم مع المنصات
            for (const auto& podium : room->podiumBounds)
            {
                float pXMin = podium.xCenter - podium.halfWidth;
                float pXMax = podium.xCenter + podium.halfWidth;
                float pZMin = podium.zCenter - podium.halfDepth;
                float pZMax = podium.zCenter + podium.halfDepth;

                bool insideX = nextPos.x > pXMin - podiumMargin && nextPos.x < pXMax + podiumMargin;
                bool insideZ = nextPos.z > pZMin - podiumMargin && nextPos.z < pZMax + podiumMargin;

                if (insideX && insideZ)
                {
                    float distLeft = std::abs(nextPos.x - (pXMin - podiumMargin));
                    float distRight = std::abs(nextPos.x - (pXMax + podiumMargin));
                    float distFront = std::abs(nextPos.z - (pZMax + podiumMargin));
                    float distBack = std::abs(nextPos.z - (pZMin - podiumMargin));

                    float minDist = std::min({ distLeft, distRight, distFront, distBack });

                    if (minDist == distLeft)
                        nextPos.x = pXMin - podiumMargin;
                    else if (minDist == distRight)
                        nextPos.x = pXMax + podiumMargin;
                    else if (minDist == distFront)
                        nextPos.z = pZMax + podiumMargin;
                    else if (minDist == distBack)
                        nextPos.z = pZMin - podiumMargin;
                }
            }
        }

        return nextPos;
    }

    Car* Showroom::findNearestCar(glm::vec3 playerPos)
    {
        for (auto& room : rooms)
        {
            Car* car = room->getCarNearPlayer(playerPos);
            if (car != nullptr)
            {
                return car;
            }
        }
        return nullptr;
    }
}