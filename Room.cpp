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
        float pW = 3.5f;      // نصف عرض المنصة (X)
        float pD = 4.5f;      // نصف عمق المنصة (Z)
        float pH = 0.25f;     // ارتفاع المنصة
        float spacing = 9.0f; // المسافة بين مراكز المنصات

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
    }

    void Room::draw(const glm::mat4& viewProj)
    {
        floor.render(glm::mat4(1.0f), viewProj);
        walls.render(glm::mat4(1.0f), viewProj);

        for (auto& podium : podiums)
        {
            podium.render(glm::mat4(1.0f), viewProj);
        }

        // رسم الباب (لوح متحرك)
        float doorZLocal = doorAtMaxZ ? size : -size;
        glm::vec3 hingeWorld = centerOffset + glm::vec3(-this->doorWidth / 2.0f, 0.0f, doorZLocal);

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