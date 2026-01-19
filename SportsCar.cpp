#include "SportsCar.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace Example
{

    static void addQuad(std::vector<BasicVertex>& v,
        glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
        glm::vec3 col, glm::vec3 normal)
    {
        v.push_back({ p1, col, normal });
        v.push_back({ p2, col, normal });
        v.push_back({ p3, col, normal });
        v.push_back({ p1, col, normal });
        v.push_back({ p3, col, normal });
        v.push_back({ p4, col, normal });
    }

    SportsCar::SportsCar()
        : position(0.0f), rotation(0.0f), bodyColor(0.9f, 0.1f, 0.1f)
    {
    }

    void SportsCar::create(glm::vec3 pos, float rotationY, glm::vec3 color)
    {
        position = pos;
        rotation = rotationY;
        bodyColor = color;

        buildBody();
        buildDoors();
        buildWheels();
        buildLights();
        buildWindows();
        buildDetails();
    }

    void SportsCar::buildBody()
    {
        std::vector<BasicVertex> bodyVerts;
        std::vector<BasicVertex> hoodVerts;
        std::vector<BasicVertex> roofVerts;
        std::vector<BasicVertex> rearVerts;

        float L = length / 2.0f;
        float W = width / 2.0f;
        float G = groundClearance;
        float H = height;

        glm::vec3 mainColor = bodyColor;
        glm::vec3 darkColor = bodyColor * 0.7f;
        glm::vec3 black = { 0.05f, 0.05f, 0.05f };

        glm::vec3 nRight = { 0, 0, 1 };
        glm::vec3 nLeft = { 0, 0, -1 };
        glm::vec3 nUp = { 0, 1, 0 };
        glm::vec3 nDown = { 0, -1, 0 };
        glm::vec3 nFront = { 1, 0, 0 };
        glm::vec3 nBack = { -1, 0, 0 };

        float noseHeight = G + 0.35f;
        float hoodHeight = G + 0.55f;
        float cabinBottom = G + 0.6f;
        float roofHeight = G + H;

        float noseEnd = L;
        float hoodEnd = L * 0.35f;
        float cabinStart = L * 0.05f;
        float cabinEnd = -L * 0.55f;

                        
                addQuad(bodyVerts,
            { L, G, W }, { -L, G, W }, { -L, cabinBottom, W }, { L, cabinBottom, W },
            mainColor, nRight);

                addQuad(bodyVerts,
            { -L, G, -W }, { L, G, -W }, { L, cabinBottom, -W }, { -L, cabinBottom, -W },
            mainColor, nLeft);

                addQuad(bodyVerts,
            { L, G, -W }, { -L, G, -W }, { -L, G, W }, { L, G, W },
            black, nDown);

        body = BasicShape(bodyVerts);

                        
        glm::vec3 hoodNormal = glm::normalize(glm::vec3(0.3f, 1.0f, 0.0f));

        addQuad(hoodVerts,
            { noseEnd - 0.1f, noseHeight, -W + 0.1f },
            { noseEnd - 0.1f, noseHeight, W - 0.1f },
            { hoodEnd, hoodHeight, W - 0.1f },
            { hoodEnd, hoodHeight, -W + 0.1f },
            mainColor, hoodNormal);

                addQuad(hoodVerts,
            { L, G, -W }, { L, G, W }, { L, noseHeight, W }, { L, noseHeight, -W },
            darkColor, nFront);

                addQuad(hoodVerts,
            { noseEnd, noseHeight, W }, { hoodEnd, hoodHeight, W },
            { hoodEnd, cabinBottom, W }, { noseEnd, G + 0.2f, W },
            mainColor, nRight);

        addQuad(hoodVerts,
            { hoodEnd, hoodHeight, -W }, { noseEnd, noseHeight, -W },
            { noseEnd, G + 0.2f, -W }, { hoodEnd, cabinBottom, -W },
            mainColor, nLeft);

        hood = BasicShape(hoodVerts);

                        
        float roofW = W * 0.82f;

        addQuad(roofVerts,
            { cabinStart, roofHeight, -roofW },
            { cabinStart, roofHeight, roofW },
            { cabinEnd, roofHeight - 0.1f, roofW },
            { cabinEnd, roofHeight - 0.1f, -roofW },
            mainColor * 0.95f, nUp);

        roof = BasicShape(roofVerts);

                        
        float tailHeight = G + 0.75f;

        addQuad(rearVerts,
            { cabinEnd, roofHeight - 0.1f, -roofW },
            { cabinEnd, roofHeight - 0.1f, roofW },
            { -L + 0.1f, tailHeight, W - 0.1f },
            { -L + 0.1f, tailHeight, -W + 0.1f },
            mainColor, glm::normalize(glm::vec3(-0.5f, 1.0f, 0.0f)));

        addQuad(rearVerts,
            { -L, G, W }, { -L, G, -W }, { -L, tailHeight, -W }, { -L, tailHeight, W },
            darkColor, nBack);

        rear = BasicShape(rearVerts);
    }

    void SportsCar::buildDoors()
    {
        std::vector<BasicVertex> doorVerts;

        float L = length / 2.0f;
        float W = width / 2.0f;
        float G = groundClearance;

        glm::vec3 doorColor = bodyColor * 0.9f;
        glm::vec3 doorDark = bodyColor * 0.75f;
        glm::vec3 nRight = { 0, 0, 1 };
        glm::vec3 nLeft = { 0, 0, -1 };

        float doorFront = L * 0.3f;
        float doorBack = -L * 0.35f;
        float doorBottom = G + 0.25f;
        float doorTop = G + 0.95f;
        float windowBottom = G + 0.65f;

                        
                addQuad(doorVerts,
            { doorFront, doorBottom, W + 0.01f },
            { doorBack, doorBottom, W + 0.01f },
            { doorBack, windowBottom, W + 0.01f },
            { doorFront, windowBottom, W + 0.01f },
            doorColor, nRight);

                addQuad(doorVerts,
            { doorFront, windowBottom, W + 0.01f },
            { doorFront - 0.08f, windowBottom, W + 0.01f },
            { doorFront - 0.1f, doorTop, W + 0.01f },
            { doorFront, doorTop, W + 0.01f },
            doorDark, nRight);

                addQuad(doorVerts,
            { doorBack + 0.08f, windowBottom, W + 0.01f },
            { doorBack, windowBottom, W + 0.01f },
            { doorBack, doorTop, W + 0.01f },
            { doorBack + 0.1f, doorTop, W + 0.01f },
            doorDark, nRight);

                        
                addQuad(doorVerts,
            { doorBack, doorBottom, -W - 0.01f },
            { doorFront, doorBottom, -W - 0.01f },
            { doorFront, windowBottom, -W - 0.01f },
            { doorBack, windowBottom, -W - 0.01f },
            doorColor, nLeft);

                addQuad(doorVerts,
            { doorFront - 0.08f, windowBottom, -W - 0.01f },
            { doorFront, windowBottom, -W - 0.01f },
            { doorFront, doorTop, -W - 0.01f },
            { doorFront - 0.1f, doorTop, -W - 0.01f },
            doorDark, nLeft);

                addQuad(doorVerts,
            { doorBack, windowBottom, -W - 0.01f },
            { doorBack + 0.08f, windowBottom, -W - 0.01f },
            { doorBack + 0.1f, doorTop, -W - 0.01f },
            { doorBack, doorTop, -W - 0.01f },
            doorDark, nLeft);

        doors = BasicShape(doorVerts);
    }

    void SportsCar::buildWheels()
    {
        std::vector<BasicVertex> wheelVerts;
        std::vector<BasicVertex> rimVerts;

        glm::vec3 tireColor = { 0.08f, 0.08f, 0.08f };
        glm::vec3 rimColor = { 0.8f, 0.8f, 0.85f };
        glm::vec3 rimDark = { 0.2f, 0.2f, 0.25f };

        float L = length / 2.0f;
        float W = width / 2.0f;

                float wheelRadius = 0.42f;
        float wheelWidth = 0.28f;
        float rimRadius = 0.32f;

        float wheelPositions[4][2] = {
            {L - 0.55f, -W - 0.02f},
            {L - 0.55f, W + 0.02f},
            {-L + 0.55f, -W - 0.02f},
            {-L + 0.55f, W + 0.02f}
        };

        int segments = 24;

        for (int w = 0; w < 4; w++)
        {
            float wx = wheelPositions[w][0];
            float wz = wheelPositions[w][1];
            float wy = wheelRadius;

            float zOffset = (wz > 0) ? wheelWidth / 2 : -wheelWidth / 2;
            float zInner = wz - zOffset;
            float zOuter = wz + zOffset;

            for (int i = 0; i < segments; i++)
            {
                float angle1 = (float)i / segments * 2.0f * 3.14159f;
                float angle2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

                float x1 = wx + cos(angle1) * wheelRadius;
                float y1 = wy + sin(angle1) * wheelRadius;
                float x2 = wx + cos(angle2) * wheelRadius;
                float y2 = wy + sin(angle2) * wheelRadius;

                glm::vec3 n1 = glm::normalize(glm::vec3(cos(angle1), sin(angle1), 0));
                glm::vec3 n2 = glm::normalize(glm::vec3(cos(angle2), sin(angle2), 0));

                wheelVerts.push_back({ {x1, y1, zInner}, tireColor, n1 });
                wheelVerts.push_back({ {x2, y2, zInner}, tireColor, n2 });
                wheelVerts.push_back({ {x2, y2, zOuter}, tireColor, n2 });
                wheelVerts.push_back({ {x1, y1, zInner}, tireColor, n1 });
                wheelVerts.push_back({ {x2, y2, zOuter}, tireColor, n2 });
                wheelVerts.push_back({ {x1, y1, zOuter}, tireColor, n1 });
            }

            float rimZ = (wz > 0) ? zOuter + 0.01f : zInner - 0.01f;
            glm::vec3 rimNormal = (wz > 0) ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1);

            for (int i = 0; i < segments; i++)
            {
                float angle1 = (float)i / segments * 2.0f * 3.14159f;
                float angle2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

                float x1 = wx + cos(angle1) * rimRadius;
                float y1 = wy + sin(angle1) * rimRadius;
                float x2 = wx + cos(angle2) * rimRadius;
                float y2 = wy + sin(angle2) * rimRadius;

                rimVerts.push_back({ {wx, wy, rimZ}, rimColor, rimNormal });
                rimVerts.push_back({ {x1, y1, rimZ}, rimDark, rimNormal });
                rimVerts.push_back({ {x2, y2, rimZ}, rimDark, rimNormal });
            }

                        for (int spoke = 0; spoke < 5; spoke++)
            {
                float spokeAngle = spoke * 2.0f * 3.14159f / 5.0f;
                float spokeWidth = 0.045f;

                float sx = cos(spokeAngle);
                float sy = sin(spokeAngle);

                rimVerts.push_back({ {wx + sx * 0.06f, wy + sy * 0.06f, rimZ}, rimColor, rimNormal });
                rimVerts.push_back({ {wx + sx * rimRadius - sy * spokeWidth, wy + sy * rimRadius + sx * spokeWidth, rimZ}, rimColor, rimNormal });
                rimVerts.push_back({ {wx + sx * rimRadius + sy * spokeWidth, wy + sy * rimRadius - sx * spokeWidth, rimZ}, rimColor, rimNormal });
            }
        }

        wheels = BasicShape(wheelVerts);
        wheelRims = BasicShape(rimVerts);
    }

    void SportsCar::buildLights()
    {
        std::vector<BasicVertex> headVerts;
        std::vector<BasicVertex> tailVerts;

        float L = length / 2.0f;
        float W = width / 2.0f;
        float G = groundClearance;

        glm::vec3 headlightColor = { 0.95f, 0.95f, 1.0f };
        glm::vec3 taillightColor = { 0.9f, 0.1f, 0.1f };

        glm::vec3 nFront = { 1, 0, 0 };
        glm::vec3 nBack = { -1, 0, 0 };

        float headY = G + 0.25f;
        float headH = 0.1f;
        float headW = 0.45f;

                addQuad(headVerts,
            { L + 0.02f, headY, -W + 0.1f }, { L + 0.02f, headY, -W + 0.1f + headW },
            { L + 0.02f, headY + headH, -W + 0.1f + headW }, { L + 0.02f, headY + headH, -W + 0.1f },
            headlightColor, nFront);

        addQuad(headVerts,
            { L + 0.02f, headY, W - 0.1f - headW }, { L + 0.02f, headY, W - 0.1f },
            { L + 0.02f, headY + headH, W - 0.1f }, { L + 0.02f, headY + headH, W - 0.1f - headW },
            headlightColor, nFront);

        headlights = BasicShape(headVerts);

                float tailY = G + 0.55f;
        float tailH = 0.08f;

        addQuad(tailVerts,
            { -L - 0.02f, tailY, W - 0.15f }, { -L - 0.02f, tailY, -W + 0.15f },
            { -L - 0.02f, tailY + tailH, -W + 0.15f }, { -L - 0.02f, tailY + tailH, W - 0.15f },
            taillightColor, nBack);

        taillights = BasicShape(tailVerts);
    }

    void SportsCar::buildWindows()
    {
        std::vector<BasicVertex> frontVerts;
        std::vector<BasicVertex> rearVerts;
        std::vector<BasicVertex> sideVerts;

        float L = length / 2.0f;
        float W = width / 2.0f;
        float G = groundClearance;
        float H = height;

        glm::vec3 glassColor = { 0.15f, 0.2f, 0.25f };

        float cabinBottom = G + 0.6f;
        float roofHeight = G + H;
        float cabinStart = L * 0.05f;
        float cabinEnd = -L * 0.55f;
        float roofW = W * 0.82f;

                glm::vec3 windshieldNormal = glm::normalize(glm::vec3(0.7f, 0.7f, 0.0f));

        addQuad(frontVerts,
            { L * 0.32f, cabinBottom + 0.08f, -roofW + 0.05f },
            { L * 0.32f, cabinBottom + 0.08f, roofW - 0.05f },
            { cabinStart, roofHeight - 0.02f, roofW * 0.88f },
            { cabinStart, roofHeight - 0.02f, -roofW * 0.88f },
            glassColor, windshieldNormal);

        windowFront = BasicShape(frontVerts);

                glm::vec3 rearWindowNormal = glm::normalize(glm::vec3(-0.7f, 0.7f, 0.0f));

        addQuad(rearVerts,
            { cabinEnd + 0.05f, roofHeight - 0.12f, -roofW * 0.88f },
            { cabinEnd + 0.05f, roofHeight - 0.12f, roofW * 0.88f },
            { -L * 0.68f, G + 0.78f, roofW - 0.1f },
            { -L * 0.68f, G + 0.78f, -roofW + 0.1f },
            glassColor, rearWindowNormal);

        windowRear = BasicShape(rearVerts);

                        
        float doorFront = L * 0.3f;
        float doorBack = -L * 0.35f;
        float windowBottom = G + 0.68f;
        float doorTop = G + 0.95f;

        glm::vec3 nLeft = { 0, 0, -1 };
        glm::vec3 nRight = { 0, 0, 1 };

                addQuad(sideVerts,
            { doorFront - 0.1f, windowBottom + 0.02f, W + 0.02f },
            { doorBack + 0.12f, windowBottom + 0.02f, W + 0.02f },
            { doorBack + 0.12f, doorTop - 0.05f, W + 0.02f },
            { doorFront - 0.12f, doorTop - 0.05f, W + 0.02f },
            glassColor, nRight);

                addQuad(sideVerts,
            { doorBack + 0.12f, windowBottom + 0.02f, -W - 0.02f },
            { doorFront - 0.1f, windowBottom + 0.02f, -W - 0.02f },
            { doorFront - 0.12f, doorTop - 0.05f, -W - 0.02f },
            { doorBack + 0.12f, doorTop - 0.05f, -W - 0.02f },
            glassColor, nLeft);

        windowsSide = BasicShape(sideVerts);
    }

    void SportsCar::buildDetails()
    {
        std::vector<BasicVertex> grilleVerts;
        std::vector<BasicVertex> mirrorVerts;
        std::vector<BasicVertex> spoilerVerts;
        std::vector<BasicVertex> handleVerts;

        float L = length / 2.0f;
        float W = width / 2.0f;
        float G = groundClearance;

        glm::vec3 black = { 0.05f, 0.05f, 0.05f };
        glm::vec3 chrome = { 0.75f, 0.75f, 0.8f };

        glm::vec3 nFront = { 1, 0, 0 };
        glm::vec3 nUp = { 0, 1, 0 };
        glm::vec3 nLeft = { 0, 0, -1 };
        glm::vec3 nRight = { 0, 0, 1 };

                addQuad(grilleVerts,
            { L + 0.03f, G + 0.08f, -0.55f }, { L + 0.03f, G + 0.08f, 0.55f },
            { L + 0.03f, G + 0.22f, 0.5f }, { L + 0.03f, G + 0.22f, -0.5f },
            black, nFront);

        grille = BasicShape(grilleVerts);

                float mirrorY = G + 0.95f;
        float mirrorX = L * 0.12f;

        addQuad(mirrorVerts,
            { mirrorX, mirrorY, -W - 0.12f }, { mirrorX + 0.08f, mirrorY, -W - 0.12f },
            { mirrorX + 0.08f, mirrorY + 0.07f, -W - 0.12f }, { mirrorX, mirrorY + 0.07f, -W - 0.12f },
            black, nLeft);

        addQuad(mirrorVerts,
            { mirrorX + 0.08f, mirrorY, W + 0.12f }, { mirrorX, mirrorY, W + 0.12f },
            { mirrorX, mirrorY + 0.07f, W + 0.12f }, { mirrorX + 0.08f, mirrorY + 0.07f, W + 0.12f },
            black, nRight);

        mirrors = BasicShape(mirrorVerts);

                float spoilerY = G + 0.8f;
        float spoilerX = -L + 0.22f;

        addQuad(spoilerVerts,
            { spoilerX, spoilerY, -W + 0.32f }, { spoilerX, spoilerY, -W + 0.38f },
            { spoilerX, spoilerY + 0.22f, -W + 0.38f }, { spoilerX, spoilerY + 0.22f, -W + 0.32f },
            black, nLeft);

        addQuad(spoilerVerts,
            { spoilerX, spoilerY, W - 0.38f }, { spoilerX, spoilerY, W - 0.32f },
            { spoilerX, spoilerY + 0.22f, W - 0.32f }, { spoilerX, spoilerY + 0.22f, W - 0.38f },
            black, nRight);

        addQuad(spoilerVerts,
            { spoilerX - 0.18f, spoilerY + 0.2f, -W + 0.28f },
            { spoilerX - 0.18f, spoilerY + 0.2f, W - 0.28f },
            { spoilerX + 0.12f, spoilerY + 0.24f, W - 0.28f },
            { spoilerX + 0.12f, spoilerY + 0.24f, -W + 0.28f },
            bodyColor, nUp);

        spoiler = BasicShape(spoilerVerts);

                float handleY = G + 0.72f;
        float handleX = -0.05f;
        float handleLen = 0.12f;

        addQuad(handleVerts,
            { handleX, handleY, W + 0.02f }, { handleX + handleLen, handleY, W + 0.02f },
            { handleX + handleLen, handleY + 0.03f, W + 0.02f }, { handleX, handleY + 0.03f, W + 0.02f },
            chrome, nRight);

        addQuad(handleVerts,
            { handleX + handleLen, handleY, -W - 0.02f }, { handleX, handleY, -W - 0.02f },
            { handleX, handleY + 0.03f, -W - 0.02f }, { handleX + handleLen, handleY + 0.03f, -W - 0.02f },
            chrome, nLeft);

        doorHandles = BasicShape(handleVerts);
    }

    void SportsCar::render(const glm::mat4& viewProj)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));

        body.render(model, viewProj);
        hood.render(model, viewProj);
        roof.render(model, viewProj);
        rear.render(model, viewProj);
        doors.render(model, viewProj);

        wheels.render(model, viewProj);
        wheelRims.render(model, viewProj);

        headlights.render(model, viewProj);
        taillights.render(model, viewProj);

        grille.render(model, viewProj);
        mirrors.render(model, viewProj);
        spoiler.render(model, viewProj);
        doorHandles.render(model, viewProj);

                windowFront.render(model, viewProj, 0.45f);
        windowRear.render(model, viewProj, 0.45f);
        windowsSide.render(model, viewProj, 0.45f);
    }

}