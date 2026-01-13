#include "Room.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Example {
    void Room::create(std::string roomName, glm::vec3 position, float roomSize, glm::vec3 color, bool isDoorAtMaxZ) {
        this->name = roomName;
        this->centerOffset = position;
        this->size = roomSize;
        this->doorAtMaxZ = isDoorAtMaxZ;

        float h = 7.0f;
        float doorWidth = 5.0f;
        std::vector<BasicVertex> floorV, wallV, podiumV, lightV;
        auto addPos = [&](glm::vec3 p) { return p + centerOffset; };

        // 1. الأرضية والمنصة
        float pS = size * 0.5f; // حجم المنصة
        float pH = 0.15f;       // ارتفاع المنصة
        glm::vec3 pColor = { 0.8f, 0.8f, 0.8f }; // رمادي فاتح للمنصة

        // أرضية الغرفة
        floorV.push_back({ addPos({-size, 0, -size}), color * 0.2f });
        floorV.push_back({ addPos({ size, 0, -size}), color * 0.2f });
        floorV.push_back({ addPos({ size, 0,  size}), color * 0.2f });
        floorV.push_back({ addPos({-size, 0, -size}), color * 0.2f });
        floorV.push_back({ addPos({ size, 0,  size}), color * 0.2f });
        floorV.push_back({ addPos({-size, 0,  size}), color * 0.2f });
        floor = BasicShape(floorV);

        // سطح المنصة
        podiumV.push_back({ addPos({-pS, pH, -pS}), pColor });
        podiumV.push_back({ addPos({ pS, pH, -pS}), pColor });
        podiumV.push_back({ addPos({ pS, pH,  pS}), pColor });
        podiumV.push_back({ addPos({-pS, pH, -pS}), pColor });
        podiumV.push_back({ addPos({ pS, pH,  pS}), pColor });
        podiumV.push_back({ addPos({-pS, pH,  pS}), pColor });
        podium = BasicShape(podiumV);

        // 2. إضاءة السقف (مربع أبيض في السقف)
        glm::vec3 lCol = { 1.0f, 1.0f, 1.0f };
        float lS = size * 0.4f;
        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({ lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({ lS, h - 0.01f,  lS}), lCol });
        lightV.push_back({ addPos({-lS, h - 0.01f, -lS}), lCol });
        lightV.push_back({ addPos({ lS, h - 0.01f,  lS}), lCol });
        lightV.push_back({ addPos({-lS, h - 0.01f,  lS}), lCol });
        ceilingLight = BasicShape(lightV);

        // 3. الجدران
        auto addWall = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
            wallV.push_back({ addPos(p1), color }); wallV.push_back({ addPos(p2), color });
            wallV.push_back({ addPos(p3), color }); wallV.push_back({ addPos(p1), color });
            wallV.push_back({ addPos(p3), color }); wallV.push_back({ addPos(p4), color });
            };
        addWall({ -size, 0, -size }, { -size, 0,  size }, { -size, h,  size }, { -size, h, -size });
        addWall({ size, 0, -size }, { size, 0,  size }, { size, h,  size }, { size, h, -size });
        float doorZ = isDoorAtMaxZ ? size : -size;
        float solidZ = isDoorAtMaxZ ? -size : size;
        addWall({ -size, 0, solidZ }, { size, 0, solidZ }, { size, h, solidZ }, { -size, h, solidZ });
        addWall({ -size, 0, doorZ }, { -doorWidth / 2, 0, doorZ }, { -doorWidth / 2, h, doorZ }, { -size, h, doorZ });
        addWall({ doorWidth / 2, 0, doorZ }, { size, 0, doorZ }, { size, h, doorZ }, { doorWidth / 2, h, doorZ });
        addWall({ -doorWidth / 2, 4.5f, doorZ }, { doorWidth / 2, 4.5f, doorZ }, { doorWidth / 2, h, doorZ }, { -doorWidth / 2, h, doorZ });
        walls = BasicShape(wallV);
    }

    void Room::draw(const glm::mat4& viewProj) {
        floor.render(glm::mat4(1.0f), viewProj);
        walls.render(glm::mat4(1.0f), viewProj);
        podium.render(glm::mat4(1.0f), viewProj);
        ceilingLight.render(glm::mat4(1.0f), viewProj);
    }
}