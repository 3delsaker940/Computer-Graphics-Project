#include "SUVCar.hpp"
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

	SUVCar::SUVCar()
		: position(0.0f), rotation(0.0f), bodyColor(0.2f, 0.25f, 0.3f)
	{
	}

	void SUVCar::create(glm::vec3 pos, float rotationY, glm::vec3 color)
	{
		position = pos;
		rotation = rotationY;
		bodyColor = color;

		buildBody();
		buildWheels();
		buildLights();
		buildWindows();
		buildDetails();
	}

	void SUVCar::buildBody()
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
		glm::vec3 plastic = { 0.15f, 0.15f, 0.15f };
		glm::vec3 nRight = { 0, 0, 1 };
		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nUp = { 0, 1, 0 };
		glm::vec3 nDown = { 0, -1, 0 };
		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };


		float hoodHeight = G + 1.0f;
		float beltLine = G + 1.1f;
		float roofHeight = G + H;

		float hoodEnd = L * 0.3f;
		float cabinEnd = -L * 0.75f;

		addQuad(bodyVerts,
			{ L, G, W }, { -L, G, W }, { -L, G + 0.15f, W }, { L, G + 0.15f, W },
			plastic, nRight);

		addQuad(bodyVerts,
			{ L, G + 0.15f, W }, { -L, G + 0.15f, W }, { -L, beltLine, W }, { L, beltLine, W },
			mainColor, nRight);

		addQuad(bodyVerts,
			{ -L, G, -W }, { L, G, -W }, { L, G + 0.15f, -W }, { -L, G + 0.15f, -W },
			plastic, nLeft);

		addQuad(bodyVerts,
			{ -L, G + 0.15f, -W }, { L, G + 0.15f, -W }, { L, beltLine, -W }, { -L, beltLine, -W },
			mainColor, nLeft);

		addQuad(bodyVerts,
			{ L, G, -W }, { -L, G, -W }, { -L, G, W }, { L, G, W },
			black, nDown);

		body = BasicShape(bodyVerts);


		addQuad(hoodVerts,
			{ L - 0.1f, hoodHeight, -W + 0.15f },
			{ L - 0.1f, hoodHeight, W - 0.15f },
			{ hoodEnd, hoodHeight + 0.05f, W - 0.15f },
			{ hoodEnd, hoodHeight + 0.05f, -W + 0.15f },
			mainColor, nUp);

		addQuad(hoodVerts,
			{ L, G + 0.4f, -W }, { L, G + 0.4f, W },
			{ L, hoodHeight, W }, { L, hoodHeight, -W },
			mainColor, nFront);

		addQuad(hoodVerts,
			{ L, G, -W }, { L, G, W }, { L, G + 0.4f, W }, { L, G + 0.4f, -W },
			plastic, nFront);

		hood = BasicShape(hoodVerts);


		float roofW = W * 0.92f;
		float cabinStart = hoodEnd - 0.15f;

		addQuad(roofVerts,
			{ cabinStart, roofHeight, -roofW },
			{ cabinStart, roofHeight, roofW },
			{ cabinEnd + 0.1f, roofHeight, roofW },
			{ cabinEnd + 0.1f, roofHeight, -roofW },
			mainColor * 0.95f, nUp);

		addQuad(roofVerts,
			{ cabinStart, beltLine, W }, { cabinEnd, beltLine, W },
			{ cabinEnd, roofHeight, W * 0.95f }, { cabinStart, roofHeight, W * 0.95f },
			mainColor, nRight);

		addQuad(roofVerts,
			{ cabinEnd, beltLine, -W }, { cabinStart, beltLine, -W },
			{ cabinStart, roofHeight, -W * 0.95f }, { cabinEnd, roofHeight, -W * 0.95f },
			mainColor, nLeft);

		roof = BasicShape(roofVerts);


		addQuad(rearVerts,
			{ -L, G + 0.15f, W }, { -L, G + 0.15f, -W },
			{ -L, roofHeight, -W * 0.95f }, { -L, roofHeight, W * 0.95f },
			mainColor, nBack);

		addQuad(rearVerts,
			{ -L, G, W }, { -L, G, -W }, { -L, G + 0.15f, -W }, { -L, G + 0.15f, W },
			plastic, nBack);

		rear = BasicShape(rearVerts);
	}

	void SUVCar::buildWheels()
	{
		std::vector<BasicVertex> wheelVerts;
		std::vector<BasicVertex> rimVerts;

		glm::vec3 tireColor = { 0.08f, 0.08f, 0.08f };
		glm::vec3 rimColor = { 0.6f, 0.6f, 0.65f };
		glm::vec3 rimDark = { 0.25f, 0.25f, 0.3f };

		float L = length / 2.0f;
		float W = width / 2.0f;

		float wheelRadius = 0.4f;
		float wheelWidth = 0.28f;
		float rimRadius = 0.28f;

		float wheelPositions[4][2] = {
			{L - 0.65f, -W - 0.03f},
			{L - 0.65f, W + 0.03f},
			{-L + 0.65f, -W - 0.03f},
			{-L + 0.65f, W + 0.03f}
		};

		int segments = 20;

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

			float rimZ = (wz > 0) ? zInner + 0.01f : zOuter - 0.01f;
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

			for (int spoke = 0; spoke < 6; spoke++)
			{
				float spokeAngle = spoke * 2.0f * 3.14159f / 6.0f;
				float spokeWidth = 0.035f;

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

	void SUVCar::buildLights()
	{
		std::vector<BasicVertex> headVerts;
		std::vector<BasicVertex> tailVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;

		glm::vec3 headlightColor = { 0.95f, 0.95f, 0.9f };
		glm::vec3 taillightColor = { 0.85f, 0.1f, 0.1f };

		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };

		float headY = G + 0.7f;
		float headH = 0.25f;
		float headW = 0.35f;

		addQuad(headVerts,
			{ L + 0.02f, headY, -W + 0.15f }, { L + 0.02f, headY, -W + 0.15f + headW },
			{ L + 0.02f, headY + headH, -W + 0.15f + headW }, { L + 0.02f, headY + headH, -W + 0.15f },
			headlightColor, nFront);

		addQuad(headVerts,
			{ L + 0.02f, headY, W - 0.15f - headW }, { L + 0.02f, headY, W - 0.15f },
			{ L + 0.02f, headY + headH, W - 0.15f }, { L + 0.02f, headY + headH, W - 0.15f - headW },
			headlightColor, nFront);

		headlights = BasicShape(headVerts);

		float tailY = G + 0.5f;
		float tailH = 0.35f;
		float tailW = 0.25f;

		addQuad(tailVerts,
			{ -L - 0.02f, tailY, -W + 0.2f + tailW }, { -L - 0.02f, tailY, -W + 0.2f },
			{ -L - 0.02f, tailY + tailH, -W + 0.2f }, { -L - 0.02f, tailY + tailH, -W + 0.2f + tailW },
			taillightColor, nBack);

		addQuad(tailVerts,
			{ -L - 0.02f, tailY, W - 0.2f }, { -L - 0.02f, tailY, W - 0.2f - tailW },
			{ -L - 0.02f, tailY + tailH, W - 0.2f - tailW }, { -L - 0.02f, tailY + tailH, W - 0.2f },
			taillightColor, nBack);

		taillights = BasicShape(tailVerts);
	}

	void SUVCar::buildWindows()
	{
		std::vector<BasicVertex> frontVerts;
		std::vector<BasicVertex> rearVerts;
		std::vector<BasicVertex> sideVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;
		float H = height;

		glm::vec3 glassColor = { 0.2f, 0.25f, 0.3f };

		float beltLine = G + 1.1f;
		float roofHeight = G + H;
		float hoodEnd = L * 0.3f;
		float cabinStart = hoodEnd - 0.15f;
		float cabinEnd = -L * 0.75f;
		float roofW = W * 0.92f;

		glm::vec3 windshieldNormal = glm::normalize(glm::vec3(0.4f, 0.9f, 0.0f));

		addQuad(frontVerts,
			{ hoodEnd + 0.05f, beltLine + 0.05f, -roofW + 0.05f },
			{ hoodEnd + 0.05f, beltLine + 0.05f, roofW - 0.05f },
			{ cabinStart, roofHeight - 0.02f, roofW - 0.08f },
			{ cabinStart, roofHeight - 0.02f, -roofW + 0.08f },
			glassColor, windshieldNormal);

		windowFront = BasicShape(frontVerts);

		glm::vec3 rearWindowNormal = glm::normalize(glm::vec3(-0.2f, 0.98f, 0.0f));

		addQuad(rearVerts,
			{ cabinEnd + 0.12f, roofHeight - 0.02f, -roofW + 0.08f },
			{ cabinEnd + 0.12f, roofHeight - 0.02f, roofW - 0.08f },
			{ cabinEnd - 0.02f, beltLine + 0.1f, roofW - 0.1f },
			{ cabinEnd - 0.02f, beltLine + 0.1f, -roofW + 0.1f },
			glassColor, rearWindowNormal);

		windowRear = BasicShape(rearVerts);

		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nRight = { 0, 0, 1 };

		addQuad(sideVerts,
			{ cabinStart - 0.02f, beltLine + 0.05f, -W + 0.02f },
			{ cabinEnd + 0.1f, beltLine + 0.05f, -W + 0.02f },
			{ cabinEnd + 0.08f, roofHeight - 0.05f, -roofW + 0.02f },
			{ cabinStart, roofHeight - 0.02f, -roofW + 0.02f },
			glassColor, nLeft);

		addQuad(sideVerts,
			{ cabinEnd + 0.1f, beltLine + 0.05f, W - 0.02f },
			{ cabinStart - 0.02f, beltLine + 0.05f, W - 0.02f },
			{ cabinStart, roofHeight - 0.02f, roofW - 0.02f },
			{ cabinEnd + 0.08f, roofHeight - 0.05f, roofW - 0.02f },
			glassColor, nRight);

		windowsSide = BasicShape(sideVerts);
	}

	void SUVCar::buildDetails()
	{
		std::vector<BasicVertex> grilleVerts;
		std::vector<BasicVertex> mirrorVerts;
		std::vector<BasicVertex> rackVerts;
		std::vector<BasicVertex> bumperVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;
		float H = height;

		glm::vec3 chrome = { 0.75f, 0.75f, 0.8f };
		glm::vec3 black = { 0.08f, 0.08f, 0.08f };
		glm::vec3 plastic = { 0.15f, 0.15f, 0.15f };

		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nUp = { 0, 1, 0 };
		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nRight = { 0, 0, 1 };
		glm::vec3 nBack = { -1, 0, 0 };

		float grilleTop = G + 0.95f;
		float grilleBottom = G + 0.45f;

		addQuad(grilleVerts,
			{ L + 0.03f, grilleBottom, -0.55f }, { L + 0.03f, grilleBottom, 0.55f },
			{ L + 0.03f, grilleTop, 0.5f }, { L + 0.03f, grilleTop, -0.5f },
			black, nFront);

		for (int i = 0; i < 5; i++)
		{
			float y = grilleBottom + (grilleTop - grilleBottom) * (i + 1) / 6.0f;
			addQuad(grilleVerts,
				{ L + 0.04f, y - 0.012f, -0.52f }, { L + 0.04f, y - 0.012f, 0.52f },
				{ L + 0.04f, y + 0.012f, 0.52f }, { L + 0.04f, y + 0.012f, -0.52f },
				chrome, nFront);
		}

		grille = BasicShape(grilleVerts);

		float mirrorY = G + 1.3f;
		float mirrorX = L * 0.2f;

		addQuad(mirrorVerts,
			{ mirrorX, mirrorY, -W - 0.12f }, { mirrorX + 0.1f, mirrorY, -W - 0.12f },
			{ mirrorX + 0.1f, mirrorY + 0.12f, -W - 0.12f }, { mirrorX, mirrorY + 0.12f, -W - 0.12f },
			black, nLeft);

		addQuad(mirrorVerts,
			{ mirrorX + 0.1f, mirrorY, W + 0.12f }, { mirrorX, mirrorY, W + 0.12f },
			{ mirrorX, mirrorY + 0.12f, W + 0.12f }, { mirrorX + 0.1f, mirrorY + 0.12f, W + 0.12f },
			black, nRight);

		mirrors = BasicShape(mirrorVerts);

		float roofHeight = G + H;
		float rackLen = length * 0.5f;
		float cabinStart = L * 0.15f;

		addQuad(rackVerts,
			{ cabinStart, roofHeight + 0.02f, -W * 0.85f },
			{ cabinStart - rackLen, roofHeight + 0.02f, -W * 0.85f },
			{ cabinStart - rackLen, roofHeight + 0.05f, -W * 0.85f },
			{ cabinStart, roofHeight + 0.05f, -W * 0.85f },
			chrome, nUp);

		addQuad(rackVerts,
			{ cabinStart - rackLen, roofHeight + 0.02f, W * 0.85f },
			{ cabinStart, roofHeight + 0.02f, W * 0.85f },
			{ cabinStart, roofHeight + 0.05f, W * 0.85f },
			{ cabinStart - rackLen, roofHeight + 0.05f, W * 0.85f },
			chrome, nUp);

		for (int i = 0; i < 3; i++)
		{
			float x = cabinStart - 0.2f - i * (rackLen - 0.4f) / 2.0f;
			addQuad(rackVerts,
				{ x - 0.03f, roofHeight + 0.05f, -W * 0.85f },
				{ x + 0.03f, roofHeight + 0.05f, -W * 0.85f },
				{ x + 0.03f, roofHeight + 0.05f, W * 0.85f },
				{ x - 0.03f, roofHeight + 0.05f, W * 0.85f },
				black, nUp);
		}

		roofRack = BasicShape(rackVerts);

		addQuad(bumperVerts,
			{ L + 0.05f, G, -W }, { L + 0.05f, G, W },
			{ L + 0.05f, G + 0.15f, W }, { L + 0.05f, G + 0.15f, -W },
			plastic, nFront);

		addQuad(bumperVerts,
			{ -L - 0.05f, G, W }, { -L - 0.05f, G, -W },
			{ -L - 0.05f, G + 0.15f, -W }, { -L - 0.05f, G + 0.15f, W },
			plastic, nBack);

		bumpers = BasicShape(bumperVerts);
	}

	void SUVCar::render(const glm::mat4& viewProj)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));

		body.render(model, viewProj);
		hood.render(model, viewProj);
		roof.render(model, viewProj);
		rear.render(model, viewProj);

		wheels.render(model, viewProj);
		wheelRims.render(model, viewProj);

		headlights.render(model, viewProj);
		taillights.render(model, viewProj);

		grille.render(model, viewProj);
		mirrors.render(model, viewProj);
		roofRack.render(model, viewProj);
		bumpers.render(model, viewProj);

		windowFront.render(model, viewProj, 0.45f);
		windowRear.render(model, viewProj, 0.45f);
		windowsSide.render(model, viewProj, 0.45f);
	}

}