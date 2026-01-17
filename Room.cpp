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
        this->doorWidth = 5.0f;    // أو اتركها كما في Room.hpp ولا تكتبها هنا
        this->doorHeight = 4.5f;;
        std::vector<BasicVertex> floorV, wallV, lightV;
        auto addPos = [&](glm::vec3 p) { return p + centerOffset; };

        // ═══════════════════════════════════════════════════════════
        // دالة مساعدة لإضافة مربع مع Normal صحيح
        // ═══════════════════════════════════════════════════════════
        auto addQuadWithNormal = [](std::vector<BasicVertex>& verts,
            glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
            glm::vec3 col)
            {
                // حساب Normal من أول 3 نقاط
                glm::vec3 edge1 = p2 - p1;
                glm::vec3 edge2 = p3 - p1;
                glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

                verts.push_back({ p1, col, normal });
                verts.push_back({ p2, col, normal });
                verts.push_back({ p3, col, normal });
                verts.push_back({ p1, col, normal });
                verts.push_back({ p3, col, normal });
                verts.push_back({ p4, col, normal });
            };

        auto addQuadLocal = [&](std::vector<BasicVertex>& v,
            glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
            glm::vec3 col)
            {
                v.push_back({ addPos(p1), col });
                v.push_back({ addPos(p2), col });
                v.push_back({ addPos(p3), col });
                v.push_back({ addPos(p1), col });
                v.push_back({ addPos(p3), col });
                v.push_back({ addPos(p4), col });
            };

        auto addBoxLocal = [&](std::vector<BasicVertex>& v,
            glm::vec3 mn, glm::vec3 mx, glm::vec3 col)
            {
                // 6 faces box using quads (local coords)
                glm::vec3 a(mn.x, mn.y, mn.z);
                glm::vec3 b(mx.x, mn.y, mn.z);
                glm::vec3 c(mx.x, mx.y, mn.z);
                glm::vec3 d(mn.x, mx.y, mn.z);

                glm::vec3 e(mn.x, mn.y, mx.z);
                glm::vec3 f(mx.x, mn.y, mx.z);
                glm::vec3 g(mx.x, mx.y, mx.z);
                glm::vec3 h(mn.x, mx.y, mx.z);

                // Z- face
                addQuadLocal(v, a, b, c, d, col);
                // Z+ face
                addQuadLocal(v, f, e, h, g, col);
                // X- face
                addQuadLocal(v, e, a, d, h, col);
                // X+ face
                addQuadLocal(v, b, f, g, c, col);
                // Y+ face
                addQuadLocal(v, d, c, g, h, col);
                // Y- face
                addQuadLocal(v, e, f, b, a, col);
            };

        // ═══════════════════════════════════════════════════════════
        // 1. الأرضية
        // ═══════════════════════════════════════════════════════════
        glm::vec3 floorColor = color * 0.3f;
        glm::vec3 upNormal = { 0, 1, 0 };

        floorV.push_back({ addPos({-size, 0, -size}), floorColor, upNormal });
        floorV.push_back({ addPos({size, 0, -size}), floorColor, upNormal });
        floorV.push_back({ addPos({size, 0, size}), floorColor, upNormal });
        floorV.push_back({ addPos({-size, 0, -size}), floorColor, upNormal });
        floorV.push_back({ addPos({size, 0, size}), floorColor, upNormal });
        floorV.push_back({ addPos({-size, 0, size}), floorColor, upNormal });
        floor = BasicShape(floorV);

        // ═══════════════════════════════════════════════════════════
        // 2. المنصات الثلاث 🚗🚗🚗
        // ═══════════════════════════════════════════════════════════
        float pW = 4.2f;      // نصف عرض المنصة (X)
        float pD = 5.2f;      // نصف عمق المنصة (Z)
        float pH = 0.39f;     // ارتفاع المنصة
        float spacing = 10.5f; // المسافة بين مراكز المنصات

        // موقع المنصات على محور Z (بعيداً عن الباب)
        float pZ = isDoorAtMaxZ ? -size * 0.35f : size * 0.35f;

        // ألوان المنصة
        glm::vec3 pTopColor = { 0.75f, 0.75f, 0.78f };
        glm::vec3 pSideColor = { 0.45f, 0.45f, 0.50f };
        glm::vec3 pEdgeColor = { 0.3f, 0.3f, 0.35f };

        // Normals للمنصة
        glm::vec3 nUp = { 0, 1, 0 };
        glm::vec3 nFront = { 0, 0, 1 };
        glm::vec3 nBack = { 0, 0, -1 };
        glm::vec3 nLeft = { -1, 0, 0 };
        glm::vec3 nRight = { 1, 0, 0 };

        float podiumPositions[3] = { -spacing, 0.0f, spacing };

        podiums.clear();
        podiumBounds.clear();

        for (int i = 0; i < 3; i++)
        {
            float pX = podiumPositions[i];
            std::vector<BasicVertex> podiumV;

            // السطح العلوي (Top Face)
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pTopColor, nUp });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pTopColor, nUp });

            // الجانب الأمامي (Front Face - Z+)
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nFront });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nFront });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nFront });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nFront });

            // الجانب الخلفي (Back Face - Z-)
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nBack });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nBack });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nBack });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nBack });

            // الجانب الأيسر (Left Face - X-)
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ + pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, 0,  pZ - pD}), pSideColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ + pD}), pEdgeColor, nLeft });
            podiumV.push_back({ addPos({pX - pW, pH, pZ - pD}), pEdgeColor, nLeft });

            // الجانب الأيمن (Right Face - X+)
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ - pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nRight });
            podiumV.push_back({ addPos({pX + pW, 0,  pZ + pD}), pSideColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ - pD}), pEdgeColor, nRight });
            podiumV.push_back({ addPos({pX + pW, pH, pZ + pD}), pEdgeColor, nRight });

            podiums.push_back(BasicShape(podiumV));

            podiumBounds.push_back({
                centerOffset.x + pX,
                centerOffset.z + pZ,
                pW,
                pD
                });
        }

        // ═══════════════════════════════════════════════════════════
        // 3. إضاءة السقف 💡
        // ═══════════════════════════════════════════════════════════
        glm::vec3 lCol = { 1.0f, 1.0f, 0.95f };
        glm::vec3 downNormal = { 0, -1, 0 };
        float lS = size * 0.35f;

        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, h - 0.01f, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, h - 0.01f, lS}), lCol, downNormal });
        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol, downNormal });
        lightV.push_back({ addPos({lS, h - 0.01f, lS}), lCol, downNormal });
        lightV.push_back({ addPos({-lS, h - 0.01f, lS}), lCol, downNormal });
        ceilingLight = BasicShape(lightV);

        // ═══════════════════════════════════════════════════════════
        // 4. الجدران 🧱
        // ═══════════════════════════════════════════════════════════

        // الجدار الأيسر (X = -size) - Normal يشير لليمين (+X)
        glm::vec3 wallNormalRight = { 1, 0, 0 };
        addQuadWithNormal(wallV,
            addPos({ -size, 0, -size }), addPos({ -size, 0, size }),
            addPos({ -size, h, size }), addPos({ -size, h, -size }),
            color);

        // الجدار الأيمن (X = +size) - Normal يشير لليسار (-X)
        glm::vec3 wallNormalLeft = { -1, 0, 0 };
        addQuadWithNormal(wallV,
            addPos({ size, 0, size }), addPos({ size, 0, -size }),
            addPos({ size, h, -size }), addPos({ size, h, size }),
            color);

        // الجدار المصمت (بدون باب)
        float doorZ = isDoorAtMaxZ ? size : -size;
        float solidZ = isDoorAtMaxZ ? -size : size;
        glm::vec3 solidWallNormal = isDoorAtMaxZ ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1);

        addQuadWithNormal(wallV,
            addPos({ -size, 0, solidZ }), addPos({ size, 0, solidZ }),
            addPos({ size, h, solidZ }), addPos({ -size, h, solidZ }),
            color);

        // جدار الباب (3 أجزاء)
        glm::vec3 doorWallNormal = isDoorAtMaxZ ? glm::vec3(0, 0, -1) : glm::vec3(0, 0, 1);

        // الجزء الأيسر من الباب
        addQuadWithNormal(wallV,
            addPos({ -size, 0, doorZ }), addPos({ -this->doorWidth / 2, 0, doorZ }),
            addPos({ -this->doorWidth / 2, h, doorZ }), addPos({ -size, h, doorZ }),
            color);

        // الجزء الأيمن من الباب
        addQuadWithNormal(wallV,
            addPos({ this->doorWidth / 2, 0, doorZ }), addPos({ size, 0, doorZ }),
            addPos({ size, h, doorZ }), addPos({ this->doorWidth / 2, h, doorZ }),
            color);

        // الجزء العلوي فوق الباب
        addQuadWithNormal(wallV,
            addPos({ -this->doorWidth / 2, 4.5f, doorZ }), addPos({ this->doorWidth / 2, 4.5f, doorZ }),
            addPos({ this->doorWidth / 2, h, doorZ }), addPos({ -this->doorWidth / 2, h, doorZ }),
            color);

        walls = BasicShape(wallV);

        // =====================
// Door panel geometry
// =====================
        float doorHeight = 4.5f; // مثل فتحة الباب عندك
        glm::vec3 doorColor = color * 0.85f;

        // الباب عند z = doorZ (محلياً ضمن الغرفة)
        // سنبني الباب في نظام إحداثيات محلي حول المفصلة: x من 0..this->doorWidth و z=0
        // ثم سنضعه في العالم عبر مصفوفة Transform أثناء الرسم.
        std::vector<BasicVertex> doorV;

        glm::vec3 nDoor = { 0, 0, 1 }; // Normal افتراضي (لن ندقق بالإضاءة هنا)

        // مستطيل الباب (لوح واحد) - متموضع عند المفصلة (0)
        doorV.push_back({ {0.0f,       0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  doorHeight,0.0f}, doorColor, nDoor });
        doorV.push_back({ {0.0f,       0.0f,      0.0f}, doorColor, nDoor });
        doorV.push_back({ {this->doorWidth,  doorHeight,0.0f}, doorColor, nDoor });
        doorV.push_back({ {0.0f,       doorHeight,0.0f}, doorColor, nDoor });

        doorPanel = BasicShape(doorV);

        // إعداد زاوية الفتح حسب جهة الباب
        float openAngle = doorAtMaxZ ? -90.0f : 90.0f;
        doorTargetAngle = doorOpen ? openAngle : 0.0f;
        doorAngle = doorTargetAngle;

        

        // ===============================
 // Exterior facade v2 (NO z-fighting, NO covering the door hole)
 // ===============================
        exteriorDecor.clear();

        std::vector<BasicVertex> panelV, frameV, signV, accentV;

        // مكان جدار الباب
        float doorPlaneZ = (doorAtMaxZ ? size : -size);
        float faceSign = (doorAtMaxZ ? 1.0f : -1.0f);

        // نرسم لوحة الواجهة خلف الإطار قليلاً (لتجنب z-fighting)
        // والإطار يكون أقرب للكاميرا (أمامي أكثر)
        float panelZ = doorPlaneZ + faceSign * 0.10f;
        float frameCenterZ = doorPlaneZ + faceSign * 0.18f;

        // فتحة الباب
        float DW = this->doorWidth;     // 5.0 عادة
        float DH = this->doorHeight;    // 4.5 عادة
        float sideGap = 0.08f;          // فراغ بسيط حول الفتحة
        float topY = 6.8f;              // ارتفاع الواجهة

        // ألوان
        glm::vec3 baseCol = { 0.10f, 0.10f, 0.12f };
        glm::vec3 accent = { 1.0f, 1.0f, 1.0f };

        if (name == "Luxury")   accent = { 0.90f, 0.75f, 0.25f };
        if (name == "Electric") accent = { 0.20f, 0.90f, 1.00f };
        if (name == "Sports")   accent = { 0.90f, 0.12f, 0.12f };
        if (name == "Family")   accent = { 0.95f, 0.80f, 0.55f };

        // ---- Panels حول فتحة الباب (بدون تغطية الفتحة) ----

        // يسار الفتحة
        addQuadLocal(panelV,
            { -size + 0.02f, 0.0f, panelZ },
            { -DW / 2 - sideGap, 0.0f, panelZ },
            { -DW / 2 - sideGap, topY, panelZ },
            { -size + 0.02f, topY, panelZ },
            baseCol);

        // يمين الفتحة
        addQuadLocal(panelV,
            { DW / 2 + sideGap, 0.0f, panelZ },
            { size - 0.02f, 0.0f, panelZ },
            { size - 0.02f, topY, panelZ },
            { DW / 2 + sideGap, topY, panelZ },
            baseCol);

        // فوق الفتحة
        addQuadLocal(panelV,
            { -DW / 2 - sideGap, DH, panelZ },
            { DW / 2 + sideGap, DH, panelZ },
            { DW / 2 + sideGap, topY, panelZ },
            { -DW / 2 - sideGap, topY, panelZ },
            baseCol * 1.05f);

        // ---- Frame 3D حول الباب ----
        float frameThickness = 0.25f;
        float z1 = frameCenterZ - faceSign * 0.08f;
        float z2 = frameCenterZ + faceSign * 0.22f;

        // يسار الإطار
        addBoxLocal(frameV,
            { -DW / 2 - frameThickness, 0.0f, z1 },
            { -DW / 2,                 DH,   z2 },
            accent * 0.85f);

        // يمين الإطار
        addBoxLocal(frameV,
            { DW / 2,                 0.0f, z1 },
            { DW / 2 + frameThickness, DH,   z2 },
            accent * 0.85f);

        // أعلى الإطار
        addBoxLocal(frameV,
            { -DW / 2 - frameThickness, DH, z1 },
            { DW / 2 + frameThickness, DH + frameThickness, z2 },
            accent);

        // ---- Sign فوق الباب ----
        float signH1 = topY + 0.10f;
        float signH2 = topY + 0.90f;
        float signPad = 1.2f;

        float signZ = frameCenterZ + faceSign * 0.03f;

        addQuadLocal(signV,
            { -DW / 2 - signPad, signH1, signZ },
            { DW / 2 + signPad, signH1, signZ },
            { DW / 2 + signPad, signH2, signZ },
            { -DW / 2 - signPad, signH2, signZ },
            accent * 0.55f);

        // ---- Accent بسيط لكل صالة (بدون تخريب الفتحة) ----
        if (name == "Electric")
        {
            // شريطان Neon عموديان على الطرفين
            float w = 0.14f;
            float xEdge = size - 0.30f;
            addQuadLocal(accentV, { xEdge - w, 0.4f, panelZ + faceSign * 0.02f }, { xEdge, 0.4f, panelZ + faceSign * 0.02f },
                { xEdge, topY - 0.2f, panelZ + faceSign * 0.02f }, { xEdge - w, topY - 0.2f, panelZ + faceSign * 0.02f }, accent);

            addQuadLocal(accentV, { -xEdge, 0.4f, panelZ + faceSign * 0.02f }, { -xEdge + w, 0.4f, panelZ + faceSign * 0.02f },
                { -xEdge + w, topY - 0.2f, panelZ + faceSign * 0.02f }, { -xEdge, topY - 0.2f, panelZ + faceSign * 0.02f }, accent);
        }
        else if (name == "Sports")
        {
            // خطوط مائلة على اللوحة اليسرى فقط
            for (int i = 0; i < 3; i++)
            {
                float x = -size + 1.8f + i * 1.1f;
                addQuadLocal(accentV,
                    { x, 0.8f, panelZ + faceSign * 0.02f },
                    { x + 0.35f, 0.8f, panelZ + faceSign * 0.02f },
                    { x + 2.1f, 5.8f, panelZ + faceSign * 0.02f },
                    { x + 1.7f, 5.8f, panelZ + faceSign * 0.02f },
                    accent * 0.9f);
            }
        }
        else if (name == "Family")
        {
            // شريط أفقي دافئ فوق لوحة الاسم
            addQuadLocal(accentV,
                { -6.5f, topY + 0.95f, signZ },
                { 6.5f, topY + 0.95f, signZ },
                { 6.5f, topY + 1.10f, signZ },
                { -6.5f, topY + 1.10f, signZ },
                accent * 0.7f);
        }

        // خزّن الأشكال
        exteriorDecor.emplace_back(panelV);
        exteriorDecor.emplace_back(frameV);
        exteriorDecor.emplace_back(signV);
        exteriorDecor.emplace_back(accentV);


        // ===============================
// Decorations (simple per room)
// ===============================
        decorations.clear();

        auto addQuad = [&](std::vector<BasicVertex>& v,
            glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
            glm::vec3 col)
            {
                v.push_back({ a, col });
                v.push_back({ b, col });
                v.push_back({ c, col });
                v.push_back({ a, col });
                v.push_back({ c, col });
                v.push_back({ d, col });
            };

        // ديكور حسب نوع الصالة
        if (roomName == "Luxury")
        {
            // سجادة وسطية داكنة + إطار ذهبي
            std::vector<BasicVertex> rugV, goldV;
            float y = 0.02f;
            float rug = size * 0.75f;

            glm::vec3 rugCol = { 0.08f, 0.08f, 0.10f };
            glm::vec3 goldCol = { 0.9f, 0.75f, 0.25f };

            addQuad(rugV, addPos({ -rug, y, -rug }), addPos({ rug, y, -rug }),
                addPos({ rug, y,  rug }), addPos({ -rug, y,  rug }), rugCol);

            float bw = 0.35f;
            // إطار ذهبي (4 شرائط)
            addQuad(goldV, addPos({ -rug, y + 0.001f, -rug }), addPos({ rug, y + 0.001f, -rug }),
                addPos({ rug, y + 0.001f, -rug + bw }), addPos({ -rug, y + 0.001f, -rug + bw }), goldCol);

            addQuad(goldV, addPos({ -rug, y + 0.001f, rug - bw }), addPos({ rug, y + 0.001f, rug - bw }),
                addPos({ rug, y + 0.001f, rug }), addPos({ -rug, y + 0.001f, rug }), goldCol);

            addQuad(goldV, addPos({ -rug, y + 0.001f, -rug }), addPos({ -rug + bw, y + 0.001f, -rug }),
                addPos({ -rug + bw, y + 0.001f, rug }), addPos({ -rug, y + 0.001f, rug }), goldCol);

            addQuad(goldV, addPos({ rug - bw, y + 0.001f, -rug }), addPos({ rug, y + 0.001f, -rug }),
                addPos({ rug, y + 0.001f, rug }), addPos({ rug - bw, y + 0.001f, rug }), goldCol);

            decorations.emplace_back(rugV);
            decorations.emplace_back(goldV);
        }
        else if (roomName == "Electric")
        {
            // خطوط نيون على الأرض (سماوي/أزرق)
            std::vector<BasicVertex> neonV;
            float y = 0.03f;
            float lineW = 0.18f;
            glm::vec3 neon = { 0.2f, 0.9f, 1.0f };

            float edge = size * 0.90f;

            // خطين طوليين
            addQuad(neonV, addPos({ -edge, y, -edge }), addPos({ -edge + lineW, y, -edge }),
                addPos({ -edge + lineW, y, edge }), addPos({ -edge, y, edge }), neon);

            addQuad(neonV, addPos({ edge - lineW, y, -edge }), addPos({ edge, y, -edge }),
                addPos({ edge, y, edge }), addPos({ edge - lineW, y, edge }), neon);

            // خطين عرضيين
            addQuad(neonV, addPos({ -edge, y, -edge }), addPos({ edge, y, -edge }),
                addPos({ edge, y, -edge + lineW }), addPos({ -edge, y, -edge + lineW }), neon);

            addQuad(neonV, addPos({ -edge, y, edge - lineW }), addPos({ edge, y, edge - lineW }),
                addPos({ edge, y, edge }), addPos({ -edge, y, edge }), neon);

            decorations.emplace_back(neonV);
        }
        else if (roomName == "Sports")
        {
            // شريط أحمر باتجاه المنصات + بانر على الجدار
            std::vector<BasicVertex> stripeV, bannerV;
            float y = 0.02f;
            glm::vec3 red = { 0.85f, 0.1f, 0.1f };
            glm::vec3 banner = { 0.08f, 0.08f, 0.08f };

            // شريط أرضي من الباب للداخل
            float sW = 2.0f;
            addQuad(stripeV,
                addPos({ -sW, y,  size * 0.95f }),
                addPos({ sW, y,  size * 0.95f }),
                addPos({ sW, y, -size * 0.95f }),
                addPos({ -sW, y, -size * 0.95f }), red);

            // بانر مستطيل على جدار جانبي (قريب من الأعلى)
            float by1 = 4.2f, by2 = 6.2f;
            addQuad(bannerV,
                addPos({ -size + 0.02f, by1, -4.0f }),
                addPos({ -size + 0.02f, by1,  4.0f }),
                addPos({ -size + 0.02f, by2,  4.0f }),
                addPos({ -size + 0.02f, by2, -4.0f }), banner);

            decorations.emplace_back(stripeV);
            decorations.emplace_back(bannerV);
        }
        else if (roomName == "Family")
        {
            // Family: سجادة دافئة + كنبة بسيطة + لوحات حائط
            std::vector<BasicVertex> rugV, sofaV, framesV;
            float y = 0.02f;

            glm::vec3 rugCol = { 0.60f, 0.50f, 0.40f };     // بني دافئ
            glm::vec3 sofaCol = { 0.20f, 0.20f, 0.22f };    // رمادي غامق
            glm::vec3 frameCol = { 0.85f, 0.85f, 0.88f };   // إطار فاتح
            glm::vec3 picCol = { 0.20f, 0.45f, 0.60f };     // لوحة ملونة

            // سجادة وسطية
            float rug = size * 0.60f;
            addQuadLocal(rugV,
                { -rug, y, -rug }, { rug, y, -rug }, { rug, y, rug }, { -rug, y, rug },
                rugCol);

            // كنبة على الجدار الأيسر (داخل الغرفة) - صندوق بسيط
            float sx1 = -size + 0.8f;
            float sx2 = -size + 2.8f;
            float sz1 = -10.5f;
            float sz2 = -4.5f;
            float sy1 = 0.0f;
            float sy2 = 1.0f;

            addBoxLocal(sofaV, { sx1, sy1, sz1 }, { sx2, sy2, sz2 }, sofaCol);

            // لوحات حائط (3 إطارات) على الجدار الأيمن
            float wx = size - 0.05f; // قرب الجدار
            float by1 = 3.2f, by2 = 4.2f;

            for (int i = 0; i < 3; i++)
            {
                float wz1 = -4.0f + i * 3.0f;
                float wz2 = wz1 + 2.0f;

                // إطار
                addQuadLocal(framesV,
                    { wx, by1, wz1 }, { wx, by1, wz2 }, { wx, by2, wz2 }, { wx, by2, wz1 },
                    frameCol);

                // داخل الإطار (الصورة)
                addQuadLocal(framesV,
                    { wx - 0.01f, by1 + 0.15f, wz1 + 0.15f },
                    { wx - 0.01f, by1 + 0.15f, wz2 - 0.15f },
                    { wx - 0.01f, by2 - 0.15f, wz2 - 0.15f },
                    { wx - 0.01f, by2 - 0.15f, wz1 + 0.15f },
                    picCol * (0.8f + 0.1f * i));
            }

            decorations.emplace_back(rugV);
            decorations.emplace_back(sofaV);
            decorations.emplace_back(framesV);
        }
    }



    void Room::draw(const glm::mat4& viewProj)
    {
        floor.render(glm::mat4(1.0f), viewProj);
        walls.render(glm::mat4(1.0f), viewProj);

        // ✅ رسم واجهة خارجية (على جدار الباب)
        for (auto& ext : exteriorDecor)
            ext.render(glm::mat4(1.0f), viewProj);

        
        for (auto& podium : podiums)
        {
            podium.render(glm::mat4(1.0f), viewProj);
        }

        for (auto& deco : decorations)
        {
            deco.render(glm::mat4(1.0f), viewProj);
        }

        // رسم الباب (لوح متحرك)
        float doorZLocal = doorAtMaxZ ? size : -size;
        float faceSign = doorAtMaxZ ? 1.0f : -1.0f;
        glm::vec3 hingeWorld = centerOffset + glm::vec3(-this->doorWidth / 2.0f, 0.0f, doorZLocal + faceSign * 0.03f);

        glm::mat4 doorModel(1.0f);
        doorModel = glm::translate(doorModel, hingeWorld);
        doorModel = glm::rotate(doorModel, glm::radians(doorAngle), glm::vec3(0, 1, 0));

        doorPanel.render(doorModel, viewProj);

        ceilingLight.render(glm::mat4(1.0f), viewProj);

        for (auto& car : cars)
        {
            car->render(viewProj);
        }
    }

    void Room::addExistingCarToPodium(int podiumIndex, Example::Car* car)
    {
        if (podiumIndex < 0 || podiumIndex >= (int)podiumBounds.size())
            return;

        const auto& podium = podiumBounds[podiumIndex];

        float podiumTopY = 0.25f;
        glm::vec3 carPos = { podium.xCenter, podiumTopY - car->getGroundClearance(), podium.zCenter };

        float carRotation = doorAtMaxZ ? 180.0f : 0.0f;
        car->setPosition(carPos);

        cars.push_back(std::unique_ptr<Car>(car));
    }

    void Room::addCarToPodium(int podiumIndex, glm::vec3 carColor, glm::vec3 interiorColor)
    {
        if (podiumIndex < 0 || podiumIndex >= (int)podiumBounds.size())
            return;

        const auto& podium = podiumBounds[podiumIndex];

        float podiumTopY = 0.25f; // نفس pH عندك للمنصات

        auto car = std::make_unique<Car>();

        // أنشئها أولاً بأي y مؤقت
        glm::vec3 carPos = { podium.xCenter, 0.0f, podium.zCenter };

        float carRotation = doorAtMaxZ ? 180.0f : 0.0f;
        car->create(carPos, carRotation, carColor, interiorColor);

        // ثم اضبط y الصحيح: سطح المنصة - groundClearance
        carPos.y = podiumTopY - car->getGroundClearance(); // 0.25 - 0.2 = 0.05
        car->setPosition(carPos);

        cars.push_back(std::move(car));
    }

    Car* Room::getCarNearPlayer(glm::vec3 playerPos)
    {
        for (auto& car : cars)
        {
            if (car->isNearDriverDoor(playerPos))
                return car.get();
        }
        return nullptr;
    }

    void Room::toggleDoor()
    {
        doorOpen = !doorOpen;
        float openAngle = doorAtMaxZ ? -90.0f : 90.0f;
        doorTargetAngle = doorOpen ? openAngle : 0.0f;
    }

    void Room::update(float dt)
    {
        // تحديث باب الغرفة
        float speedDeg = 180.0f; // درجات/ثانية
        float diff = doorTargetAngle - doorAngle;

        if (std::abs(diff) <= 0.5f)
        {
            doorAngle = doorTargetAngle;
        }
        else
        {
            float step = speedDeg * dt;
            if (diff > 0) doorAngle += std::min(step, diff);
            else          doorAngle -= std::min(step, -diff);
        }

        // ملاحظة: سنحرك السيارات من Showroom::update (حتى نطبق Collision للسيارة)
    }

    bool Room::isNearDoor(const glm::vec3& playerPos) const
    {
        float doorZLocal = doorAtMaxZ ? size : -size;
        glm::vec3 doorCenter = centerOffset + glm::vec3(0.0f, 0.0f, doorZLocal);

        float dx = std::abs(playerPos.x - doorCenter.x);
        float dz = std::abs(playerPos.z - doorCenter.z);

        // قريب من فتحة الباب
        return (dx < 4.0f && dz < 2.0f);
    }
}