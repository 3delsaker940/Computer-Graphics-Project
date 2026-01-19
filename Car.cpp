#include "Car.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

static float clampf(float v, float lo, float hi)
{
	return std::max(lo, std::min(v, hi));
}

namespace Example
{

	Car::Car() : position(0.0f), rotation(0.0f),
		bodyColor(0.7f, 0.1f, 0.1f),
		interiorColor(0.15f, 0.12f, 0.1f)
	{
	}

	void Car::create(glm::vec3 pos, float rotationY, glm::vec3 bodyCol, glm::vec3 interiorCol)
	{
		position = pos;
		baseY = pos.y;
		rotation = rotationY;
		bodyColor = bodyCol;
		interiorColor = interiorCol;

		buildExterior();
		buildDriverDoor();          
		buildInterior();
		buildWheels();
		buildLights();
		buildDetails();
	}

	static void addQuadN(std::vector<BasicVertex>& v,
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

	static void addQuadAuto(std::vector<BasicVertex>& v,
		glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
		glm::vec3 col)
	{
		glm::vec3 edge1 = p2 - p1;
		glm::vec3 edge2 = p3 - p1;
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		addQuadN(v, p1, p2, p3, p4, col, normal);
	}

	void Car::buildExterior()
	{
		std::vector<BasicVertex> bodyVerts;
		std::vector<BasicVertex> hoodVerts;
		std::vector<BasicVertex> trunkVerts;
		std::vector<BasicVertex> roofVerts;
		std::vector<BasicVertex> bumperVerts;

		glm::vec3 mainColor = bodyColor;
		glm::vec3 darkColor = bodyColor * 0.7f;
		glm::vec3 accentColor = glm::min(bodyColor * 1.1f, glm::vec3(1.0f));
		glm::vec3 blackTrim = { 0.05f, 0.05f, 0.05f };

		float L = length / 2.0f;
		float W = width / 2.0f;
		float H = height;
		float G = groundClearance;

		float hoodHeight = G + 0.75f;
		float beltLine = G + 0.95f;
		float roofStart = G + 1.35f;
		float roofHeight = G + H;

		float hoodEnd = L * 0.35f;
		float cabinStart = -L * 0.1f;
		float cabinEnd = -L * 0.7f;
		float trunkStart = cabinEnd;

		glm::vec3 nRight = { 0, 0, 1 };
		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nUp = { 0, 1, 0 };
		glm::vec3 nDown = { 0, -1, 0 };
		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };


		addQuadN(bodyVerts,
			{ L, G, W }, { -L, G, W }, { -L, beltLine, W }, { L, beltLine, W },
			mainColor, nRight);

		addQuadN(bodyVerts,
			{ hoodEnd, beltLine, W }, { cabinStart, beltLine, W },
			{ cabinStart - 0.15f, roofStart, W * 0.92f }, { hoodEnd - 0.1f, roofStart, W * 0.92f },
			darkColor, nRight);

		addQuadN(bodyVerts,
			{ cabinEnd + 0.3f, beltLine, W }, { cabinEnd, beltLine, W },
			{ cabinEnd, roofStart, W * 0.92f }, { cabinEnd + 0.3f, roofStart, W * 0.92f },
			darkColor, nRight);

		addQuadN(bodyVerts,
			{ -L, G, -W }, { L, G, -W }, { L, beltLine, -W }, { -L, beltLine, -W },
			mainColor, nLeft);

		addQuadN(bodyVerts,
			{ cabinStart, beltLine, -W }, { hoodEnd, beltLine, -W },
			{ hoodEnd - 0.1f, roofStart, -W * 0.92f }, { cabinStart - 0.15f, roofStart, -W * 0.92f },
			darkColor, nLeft);

		addQuadN(bodyVerts,
			{ cabinEnd, beltLine, -W }, { cabinEnd + 0.3f, beltLine, -W },
			{ cabinEnd + 0.3f, roofStart, -W * 0.92f }, { cabinEnd, roofStart, -W * 0.92f },
			darkColor, nLeft);

		addQuadN(bodyVerts,
			{ -L, G, -W }, { -L, G, W }, { L, G, W }, { L, G, -W },
			blackTrim, nDown);

		bodyMain = BasicShape(bodyVerts);


		glm::vec3 hoodNormal = glm::normalize(glm::vec3(0.2f, 1.0f, 0.0f));
		addQuadN(hoodVerts,
			{ L - 0.1f, hoodHeight, -W + 0.1f }, { L - 0.1f, hoodHeight, W - 0.1f },
			{ hoodEnd, beltLine - 0.05f, W - 0.1f }, { hoodEnd, beltLine - 0.05f, -W + 0.1f },
			accentColor, hoodNormal);

		addQuadN(hoodVerts,
			{ L, G + 0.3f, -W }, { L, G + 0.3f, W },
			{ L - 0.1f, hoodHeight, W - 0.1f }, { L - 0.1f, hoodHeight, -W + 0.1f },
			mainColor, nFront);

		addQuadN(hoodVerts,
			{ L, G, -W }, { L, G, W }, { L, G + 0.3f, W }, { L, G + 0.3f, -W },
			darkColor, nFront);

		bodyHood = BasicShape(hoodVerts);


		glm::vec3 trunkNormal = glm::normalize(glm::vec3(-0.2f, 1.0f, 0.0f));
		addQuadN(trunkVerts,
			{ trunkStart, roofStart - 0.15f, -W + 0.1f }, { trunkStart, roofStart - 0.15f, W - 0.1f },
			{ -L + 0.1f, beltLine + 0.1f, W - 0.1f }, { -L + 0.1f, beltLine + 0.1f, -W + 0.1f },
			accentColor, trunkNormal);

		addQuadN(trunkVerts,
			{ -L, G, W }, { -L, G, -W }, { -L, beltLine + 0.3f, -W }, { -L, beltLine + 0.3f, W },
			mainColor, nBack);

		addQuadN(trunkVerts,
			{ -L, beltLine + 0.3f, W }, { -L, beltLine + 0.3f, -W },
			{ -L + 0.1f, beltLine + 0.1f, -W + 0.1f }, { -L + 0.1f, beltLine + 0.1f, W - 0.1f },
			darkColor, nBack);

		bodyTrunk = BasicShape(trunkVerts);


		float roofW = W * 0.88f;

		addQuadN(roofVerts,
			{ cabinStart - 0.2f, roofHeight, -roofW }, { cabinStart - 0.2f, roofHeight, roofW },
			{ cabinEnd + 0.1f, roofHeight, roofW }, { cabinEnd + 0.1f, roofHeight, -roofW },
			mainColor * 0.95f, nUp);

		addQuadN(roofVerts,
			{ cabinStart - 0.2f, roofStart, -roofW }, { cabinStart - 0.2f, roofStart, roofW },
			{ cabinStart - 0.2f, roofHeight, roofW }, { cabinStart - 0.2f, roofHeight, -roofW },
			darkColor, nFront);

		addQuadN(roofVerts,
			{ cabinEnd + 0.1f, roofStart, roofW }, { cabinEnd + 0.1f, roofStart, -roofW },
			{ cabinEnd + 0.1f, roofHeight, -roofW }, { cabinEnd + 0.1f, roofHeight, roofW },
			darkColor, nBack);

		bodyRoof = BasicShape(roofVerts);


		glm::vec3 bumperColor = { 0.12f, 0.12f, 0.12f };

		addQuadN(bumperVerts,
			{ L + 0.08f, G, -W }, { L + 0.08f, G, W },
			{ L + 0.08f, G + 0.25f, W }, { L + 0.08f, G + 0.25f, -W },
			bumperColor, nFront);

		addQuadN(bumperVerts,
			{ L, G + 0.25f, -W }, { L, G + 0.25f, W },
			{ L + 0.08f, G + 0.25f, W }, { L + 0.08f, G + 0.25f, -W },
			bumperColor * 1.2f, nUp);

		addQuadN(bumperVerts,
			{ -L - 0.08f, G, W }, { -L - 0.08f, G, -W },
			{ -L - 0.08f, G + 0.25f, -W }, { -L - 0.08f, G + 0.25f, W },
			bumperColor, nBack);

		addQuadN(bumperVerts,
			{ -L, G + 0.25f, W }, { -L, G + 0.25f, -W },
			{ -L - 0.08f, G + 0.25f, -W }, { -L - 0.08f, G + 0.25f, W },
			bumperColor * 1.2f, nUp);

		bumpers = BasicShape(bumperVerts);
	}

	void Car::buildInterior()
	{
		std::vector<BasicVertex> seatVerts;
		std::vector<BasicVertex> dashVerts;
		std::vector<BasicVertex> steerVerts;
		std::vector<BasicVertex> panelVerts;
		std::vector<BasicVertex> floorVerts;
		std::vector<BasicVertex> gearVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;

		glm::vec3 seatColor = interiorColor;
		glm::vec3 seatDark = interiorColor * 0.7f;
		glm::vec3 dashColor = { 0.08f, 0.08f, 0.08f };
		glm::vec3 panelColor = { 0.12f, 0.10f, 0.08f };
		glm::vec3 carpetColor = { 0.1f, 0.1f, 0.1f };
		glm::vec3 metalColor = { 0.6f, 0.6f, 0.6f };
		glm::vec3 screenColor = { 0.1f, 0.2f, 0.3f };

		float seatY = G + 0.35f;
		float seatBackTop = G + 1.1f;
		float driverX = -0.35f;
		float passengerX = 0.35f;
		float frontSeatZ = W * 0.5f;

		glm::vec3 nUp = { 0, 1, 0 };
		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };
		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nRight = { 0, 0, 1 };

		addQuadN(floorVerts,
			{ L * 0.3f, G + 0.01f, -W + 0.15f }, { L * 0.3f, G + 0.01f, W - 0.15f },
			{ -L * 0.65f, G + 0.01f, W - 0.15f }, { -L * 0.65f, G + 0.01f, -W + 0.15f },
			carpetColor, nUp);

		floorMat = BasicShape(floorVerts);

		float seatW = 0.45f;
		float seatD = 0.5f;

		addQuadN(seatVerts,
			{ driverX - seatD / 2, seatY, -frontSeatZ - seatW / 2 },
			{ driverX + seatD / 2, seatY, -frontSeatZ - seatW / 2 },
			{ driverX + seatD / 2, seatY, -frontSeatZ + seatW / 2 },
			{ driverX - seatD / 2, seatY, -frontSeatZ + seatW / 2 },
			seatColor, nUp);

		addQuadN(seatVerts,
			{ driverX - seatD / 2, G + 0.15f, -frontSeatZ - seatW / 2 },
			{ driverX + seatD / 2, G + 0.15f, -frontSeatZ - seatW / 2 },
			{ driverX + seatD / 2, seatY, -frontSeatZ - seatW / 2 },
			{ driverX - seatD / 2, seatY, -frontSeatZ - seatW / 2 },
			seatDark, nLeft);

		addQuadN(seatVerts,
			{ driverX - seatD / 2 - 0.05f, seatY, -frontSeatZ - seatW / 2 + 0.05f },
			{ driverX - seatD / 2 - 0.05f, seatY, -frontSeatZ + seatW / 2 - 0.05f },
			{ driverX - seatD / 2 - 0.05f, seatBackTop, -frontSeatZ + seatW / 2 - 0.1f },
			{ driverX - seatD / 2 - 0.05f, seatBackTop, -frontSeatZ - seatW / 2 + 0.1f },
			seatColor, nBack);

		addQuadN(seatVerts,
			{ driverX - seatD / 2 - 0.03f, seatBackTop, -frontSeatZ - 0.12f },
			{ driverX - seatD / 2 - 0.03f, seatBackTop, -frontSeatZ + 0.12f },
			{ driverX - seatD / 2 - 0.03f, seatBackTop + 0.25f, -frontSeatZ + 0.1f },
			{ driverX - seatD / 2 - 0.03f, seatBackTop + 0.25f, -frontSeatZ - 0.1f },
			seatColor, nBack);

		addQuadN(seatVerts,
			{ passengerX - seatD / 2, seatY, frontSeatZ - seatW / 2 },
			{ passengerX + seatD / 2, seatY, frontSeatZ - seatW / 2 },
			{ passengerX + seatD / 2, seatY, frontSeatZ + seatW / 2 },
			{ passengerX - seatD / 2, seatY, frontSeatZ + seatW / 2 },
			seatColor, nUp);

		addQuadN(seatVerts,
			{ passengerX - seatD / 2 - 0.05f, seatY, frontSeatZ - seatW / 2 + 0.05f },
			{ passengerX - seatD / 2 - 0.05f, seatY, frontSeatZ + seatW / 2 - 0.05f },
			{ passengerX - seatD / 2 - 0.05f, seatBackTop, frontSeatZ + seatW / 2 - 0.1f },
			{ passengerX - seatD / 2 - 0.05f, seatBackTop, frontSeatZ - seatW / 2 + 0.1f },
			seatColor, nBack);

		float rearSeatX = -L * 0.45f;
		float rearSeatW = W * 1.5f;

		addQuadN(seatVerts,
			{ rearSeatX - 0.2f, seatY - 0.05f, -rearSeatW / 2 },
			{ rearSeatX + 0.35f, seatY - 0.05f, -rearSeatW / 2 },
			{ rearSeatX + 0.35f, seatY - 0.05f, rearSeatW / 2 },
			{ rearSeatX - 0.2f, seatY - 0.05f, rearSeatW / 2 },
			seatColor, nUp);

		addQuadN(seatVerts,
			{ rearSeatX - 0.25f, seatY - 0.05f, -rearSeatW / 2 + 0.05f },
			{ rearSeatX - 0.25f, seatY - 0.05f, rearSeatW / 2 - 0.05f },
			{ rearSeatX - 0.25f, seatBackTop - 0.1f, rearSeatW / 2 - 0.1f },
			{ rearSeatX - 0.25f, seatBackTop - 0.1f, -rearSeatW / 2 + 0.1f },
			seatColor, nBack);

		seats = BasicShape(seatVerts);

		float dashX = L * 0.25f;
		float dashTop = G + 1.0f;
		float dashBottom = G + 0.5f;

		addQuadN(dashVerts,
			{ dashX + 0.1f, dashTop, -W + 0.2f }, { dashX + 0.1f, dashTop, W - 0.2f },
			{ dashX - 0.3f, dashTop - 0.1f, W - 0.2f }, { dashX - 0.3f, dashTop - 0.1f, -W + 0.2f },
			dashColor, nUp);

		addQuadN(dashVerts,
			{ dashX - 0.3f, dashBottom, -W + 0.2f }, { dashX - 0.3f, dashBottom, W - 0.2f },
			{ dashX - 0.3f, dashTop - 0.1f, W - 0.2f }, { dashX - 0.3f, dashTop - 0.1f, -W + 0.2f },
			dashColor * 0.8f, nBack);

		addQuadN(dashVerts,
			{ dashX - 0.28f, dashBottom + 0.2f, -frontSeatZ - 0.25f },
			{ dashX - 0.28f, dashBottom + 0.2f, -frontSeatZ + 0.25f },
			{ dashX - 0.28f, dashTop - 0.2f, -frontSeatZ + 0.2f },
			{ dashX - 0.28f, dashTop - 0.2f, -frontSeatZ - 0.2f },
			screenColor, nBack);

		addQuadN(dashVerts,
			{ dashX - 0.25f, dashBottom + 0.15f, -0.2f },
			{ dashX - 0.25f, dashBottom + 0.15f, 0.2f },
			{ dashX - 0.25f, dashTop - 0.25f, 0.18f },
			{ dashX - 0.25f, dashTop - 0.25f, -0.18f },
			screenColor * 1.2f, nBack);

		dashboard = BasicShape(dashVerts);

		float steerX = dashX - 0.45f;
		float steerY = G + 0.85f;
		float steerZ = -frontSeatZ;
		float steerRadius = 0.18f;

		int segments = 16;
		glm::vec3 steerColor = { 0.08f, 0.08f, 0.08f };
		glm::vec3 steerAccent = { 0.5f, 0.5f, 0.5f };

		for (int i = 0; i < segments; i++)
		{
			float angle1 = (float)i / segments * 2.0f * 3.14159f;
			float angle2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

			float y1 = steerY + sin(angle1) * steerRadius;
			float z1 = steerZ + cos(angle1) * steerRadius;
			float y2 = steerY + sin(angle2) * steerRadius;
			float z2 = steerZ + cos(angle2) * steerRadius;

			float thickness = 0.02f;
			glm::vec3 ringNormal = { -1, 0, 0 };

			steerVerts.push_back({ {steerX, y1, z1}, steerColor, ringNormal });
			steerVerts.push_back({ {steerX, y2, z2}, steerColor, ringNormal });
			steerVerts.push_back({ {steerX - thickness, y2, z2}, steerColor, ringNormal });
			steerVerts.push_back({ {steerX, y1, z1}, steerColor, ringNormal });
			steerVerts.push_back({ {steerX - thickness, y2, z2}, steerColor, ringNormal });
			steerVerts.push_back({ {steerX - thickness, y1, z1}, steerColor, ringNormal });
		}

		addQuadN(steerVerts,
			{ steerX - 0.01f, steerY - 0.06f, steerZ - 0.06f },
			{ steerX - 0.01f, steerY - 0.06f, steerZ + 0.06f },
			{ steerX - 0.01f, steerY + 0.06f, steerZ + 0.06f },
			{ steerX - 0.01f, steerY + 0.06f, steerZ - 0.06f },
			steerAccent, nBack);

		steeringWheel = BasicShape(steerVerts);

		float gearX = dashX - 0.6f;
		float gearY = G + 0.45f;
		float gearZ = 0.0f;

		addQuadN(gearVerts,
			{ gearX - 0.08f, gearY, gearZ - 0.1f },
			{ gearX + 0.08f, gearY, gearZ - 0.1f },
			{ gearX + 0.08f, gearY, gearZ + 0.1f },
			{ gearX - 0.08f, gearY, gearZ + 0.1f },
			{ 0.05f, 0.05f, 0.05f }, nUp);

		addQuadN(gearVerts,
			{ gearX - 0.02f, gearY, gearZ - 0.02f },
			{ gearX + 0.02f, gearY, gearZ - 0.02f },
			{ gearX + 0.02f, gearY + 0.15f, gearZ - 0.02f },
			{ gearX - 0.02f, gearY + 0.15f, gearZ - 0.02f },
			metalColor, nLeft);

		addQuadN(gearVerts,
			{ gearX - 0.03f, gearY + 0.15f, gearZ - 0.03f },
			{ gearX + 0.03f, gearY + 0.15f, gearZ - 0.03f },
			{ gearX + 0.03f, gearY + 0.22f, gearZ - 0.03f },
			{ gearX - 0.03f, gearY + 0.22f, gearZ - 0.03f },
			seatColor, nLeft);

		gearShift = BasicShape(gearVerts);

		float doorPanelY = G + 0.4f;
		float doorPanelTop = G + 0.9f;

		addQuadN(panelVerts,
			{ L * 0.3f, doorPanelY, -W + 0.08f }, { -L * 0.5f, doorPanelY, -W + 0.08f },
			{ -L * 0.5f, doorPanelTop, -W + 0.08f }, { L * 0.3f, doorPanelTop, -W + 0.08f },
			panelColor, nRight);

		addQuadN(panelVerts,
			{ 0.0f, doorPanelY + 0.25f, -W + 0.08f }, { -0.4f, doorPanelY + 0.25f, -W + 0.08f },
			{ -0.4f, doorPanelY + 0.32f, -W + 0.12f }, { 0.0f, doorPanelY + 0.32f, -W + 0.12f },
			panelColor * 1.2f, nRight);

		addQuadN(panelVerts,
			{ -L * 0.5f, doorPanelY, W - 0.08f }, { L * 0.3f, doorPanelY, W - 0.08f },
			{ L * 0.3f, doorPanelTop, W - 0.08f }, { -L * 0.5f, doorPanelTop, W - 0.08f },
			panelColor, nLeft);

		interiorPanels = BasicShape(panelVerts);
	}

	void Car::buildWheels()
	{
		std::vector<BasicVertex> wheelVerts;
		std::vector<BasicVertex> rimVerts;

		glm::vec3 tireColor = { 0.08f, 0.08f, 0.08f };
		glm::vec3 rimColor = { 0.7f, 0.7f, 0.75f };
		glm::vec3 rimDark = { 0.3f, 0.3f, 0.35f };

		float L = length / 2.0f;
		float W = width / 2.0f;

		float wheelRadius = 0.35f;
		float wheelWidth = 0.22f;
		float rimRadius = 0.25f;

		float wheelPositions[4][2] = {
			{L - 0.6f, -W - 0.02f},
			{L - 0.6f, W + 0.02f},
			{-L + 0.6f, -W - 0.02f},
			{-L + 0.6f, W + 0.02f}
		};

		int segments = 16;

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
				float spokeWidth = 0.03f;

				float sx = cos(spokeAngle);
				float sy = sin(spokeAngle);

				rimVerts.push_back({ {wx + sx * 0.05f, wy + sy * 0.05f, rimZ}, rimColor, rimNormal });
				rimVerts.push_back({ {wx + sx * rimRadius - sy * spokeWidth, wy + sy * rimRadius + sx * spokeWidth, rimZ}, rimDark, rimNormal });
				rimVerts.push_back({ {wx + sx * rimRadius + sy * spokeWidth, wy + sy * rimRadius - sx * spokeWidth, rimZ}, rimDark, rimNormal });
			}
		}

		wheels = BasicShape(wheelVerts);
		wheelRims = BasicShape(rimVerts);
	}

	void Car::buildLights()
	{
		std::vector<BasicVertex> headVerts;
		std::vector<BasicVertex> tailVerts;
		std::vector<BasicVertex> indicatorVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;

		glm::vec3 headlightColor = { 0.95f, 0.95f, 0.85f };
		glm::vec3 taillightColor = { 0.8f, 0.1f, 0.1f };
		glm::vec3 indicatorColor = { 1.0f, 0.6f, 0.1f };

		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };

		float headY = G + 0.55f;
		float headH = 0.2f;
		float headW = 0.35f;

		addQuadN(headVerts,
			{ L + 0.05f, headY, -W + 0.1f }, { L + 0.05f, headY, -W + 0.1f + headW },
			{ L + 0.05f, headY + headH, -W + 0.1f + headW }, { L + 0.05f, headY + headH, -W + 0.1f },
			headlightColor, nFront);

		addQuadN(headVerts,
			{ L + 0.05f, headY, W - 0.1f - headW }, { L + 0.05f, headY, W - 0.1f },
			{ L + 0.05f, headY + headH, W - 0.1f }, { L + 0.05f, headY + headH, W - 0.1f - headW },
			headlightColor, nFront);

		headlights = BasicShape(headVerts);

		float tailY = G + 0.5f;
		float tailH = 0.25f;
		float tailW = 0.3f;

		addQuadN(tailVerts,
			{ -L - 0.05f, tailY, -W + 0.15f + tailW }, { -L - 0.05f, tailY, -W + 0.15f },
			{ -L - 0.05f, tailY + tailH, -W + 0.15f }, { -L - 0.05f, tailY + tailH, -W + 0.15f + tailW },
			taillightColor, nBack);

		addQuadN(tailVerts,
			{ -L - 0.05f, tailY, W - 0.15f }, { -L - 0.05f, tailY, W - 0.15f - tailW },
			{ -L - 0.05f, tailY + tailH, W - 0.15f - tailW }, { -L - 0.05f, tailY + tailH, W - 0.15f },
			taillightColor, nBack);

		taillights = BasicShape(tailVerts);

		addQuadN(indicatorVerts,
			{ L + 0.06f, headY + headH + 0.02f, -W + 0.08f },
			{ L + 0.06f, headY + headH + 0.02f, -W + 0.2f },
			{ L + 0.06f, headY + headH + 0.08f, -W + 0.2f },
			{ L + 0.06f, headY + headH + 0.08f, -W + 0.08f },
			indicatorColor, nFront);

		addQuadN(indicatorVerts,
			{ L + 0.06f, headY + headH + 0.02f, W - 0.2f },
			{ L + 0.06f, headY + headH + 0.02f, W - 0.08f },
			{ L + 0.06f, headY + headH + 0.08f, W - 0.08f },
			{ L + 0.06f, headY + headH + 0.08f, W - 0.2f },
			indicatorColor, nFront);

		indicators = BasicShape(indicatorVerts);
	}

	void Car::buildDetails()
	{
		std::vector<BasicVertex> handleVerts;
		std::vector<BasicVertex> grilleVerts;
		std::vector<BasicVertex> mirrorVerts;
		std::vector<BasicVertex> windowFrontVerts;
		std::vector<BasicVertex> windowRearVerts;
		std::vector<BasicVertex> windowLeftVerts;
		std::vector<BasicVertex> windowRightVerts;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;

		glm::vec3 handleColor = { 0.6f, 0.6f, 0.6f };
		glm::vec3 grilleColor = { 0.1f, 0.1f, 0.1f };
		glm::vec3 grilleBright = { 0.4f, 0.4f, 0.4f };
		glm::vec3 mirrorColor = { 0.05f, 0.05f, 0.05f };
		glm::vec3 glassColor = { 0.3f, 0.4f, 0.5f };

		glm::vec3 nFront = { 1, 0, 0 };
		glm::vec3 nBack = { -1, 0, 0 };
		glm::vec3 nLeft = { 0, 0, -1 };
		glm::vec3 nRight = { 0, 0, 1 };

		float handleY = G + 0.85f;
		float handleLen = 0.15f;

		addQuadN(handleVerts,
			{ 0.1f, handleY, -W - 0.01f }, { 0.1f + handleLen, handleY, -W - 0.01f },
			{ 0.1f + handleLen, handleY + 0.04f, -W - 0.01f }, { 0.1f, handleY + 0.04f, -W - 0.01f },
			handleColor, nLeft);

		addQuadN(handleVerts,
			{ 0.1f + handleLen, handleY, W + 0.01f }, { 0.1f, handleY, W + 0.01f },
			{ 0.1f, handleY + 0.04f, W + 0.01f }, { 0.1f + handleLen, handleY + 0.04f, W + 0.01f },
			handleColor, nRight);

		doorHandles = BasicShape(handleVerts);

		float grilleTop = G + 0.55f;
		float grilleBottom = G + 0.25f;

		addQuadN(grilleVerts,
			{ L + 0.06f, grilleBottom, -0.4f }, { L + 0.06f, grilleBottom, 0.4f },
			{ L + 0.06f, grilleTop, 0.35f }, { L + 0.06f, grilleTop, -0.35f },
			grilleColor, nFront);

		for (int i = 0; i < 4; i++)
		{
			float y = grilleBottom + (grilleTop - grilleBottom) * (i + 1) / 5.0f;
			addQuadN(grilleVerts,
				{ L + 0.07f, y - 0.01f, -0.38f }, { L + 0.07f, y - 0.01f, 0.38f },
				{ L + 0.07f, y + 0.01f, 0.38f }, { L + 0.07f, y + 0.01f, -0.38f },
				grilleBright, nFront);
		}

		grille = BasicShape(grilleVerts);

		float mirrorX = L * 0.25f;
		float mirrorY = G + 1.0f;

		addQuadN(mirrorVerts,
			{ mirrorX, mirrorY, -W - 0.12f }, { mirrorX + 0.08f, mirrorY, -W - 0.12f },
			{ mirrorX + 0.08f, mirrorY + 0.1f, -W - 0.12f }, { mirrorX, mirrorY + 0.1f, -W - 0.12f },
			mirrorColor, nLeft);

		addQuadN(mirrorVerts,
			{ mirrorX + 0.08f, mirrorY, W + 0.12f }, { mirrorX, mirrorY, W + 0.12f },
			{ mirrorX, mirrorY + 0.1f, W + 0.12f }, { mirrorX + 0.08f, mirrorY + 0.1f, W + 0.12f },
			mirrorColor, nRight);

		mirrors = BasicShape(mirrorVerts);

		float roofW = W * 0.88f;
		float beltLine = G + 0.95f;
		float roofStart = G + 1.35f;
		float hoodEnd = L * 0.35f;
		float cabinStart = -L * 0.1f;
		float cabinEnd = -L * 0.7f;

		glm::vec3 windshieldNormal = glm::normalize(glm::vec3(0.6f, 0.8f, 0.0f));

		addQuadN(windowFrontVerts,
			{ hoodEnd - 0.05f, beltLine + 0.02f, -roofW + 0.05f },
			{ hoodEnd - 0.05f, beltLine + 0.02f, roofW - 0.05f },
			{ cabinStart - 0.15f, roofStart - 0.02f, roofW - 0.1f },
			{ cabinStart - 0.15f, roofStart - 0.02f, -roofW + 0.1f },
			glassColor, windshieldNormal);

		windowFront = BasicShape(windowFrontVerts);

		glm::vec3 rearWindowNormal = glm::normalize(glm::vec3(-0.6f, 0.8f, 0.0f));

		addQuadN(windowRearVerts,
			{ cabinEnd + 0.25f, roofStart - 0.02f, -roofW + 0.1f },
			{ cabinEnd + 0.25f, roofStart - 0.02f, roofW - 0.1f },
			{ cabinEnd - 0.1f, beltLine + 0.1f, roofW - 0.15f },
			{ cabinEnd - 0.1f, beltLine + 0.1f, -roofW + 0.15f },
			glassColor, rearWindowNormal);

		windowRear = BasicShape(windowRearVerts);

		addQuadN(windowLeftVerts,
			{ cabinStart - 0.1f, beltLine + 0.05f, -W + 0.03f },
			{ cabinEnd + 0.25f, beltLine + 0.05f, -W + 0.03f },
			{ cabinEnd + 0.2f, roofStart - 0.05f, -roofW + 0.02f },
			{ cabinStart - 0.12f, roofStart - 0.05f, -roofW + 0.02f },
			glassColor, nLeft);

		windowsLeft = BasicShape(windowLeftVerts);

		addQuadN(windowRightVerts,
			{ cabinEnd + 0.25f, beltLine + 0.05f, W - 0.03f },
			{ cabinStart - 0.1f, beltLine + 0.05f, W - 0.03f },
			{ cabinStart - 0.12f, roofStart - 0.05f, roofW - 0.02f },
			{ cabinEnd + 0.2f, roofStart - 0.05f, roofW - 0.02f },
			glassColor, nRight);

		windowsRight = BasicShape(windowRightVerts);
	}

	void Car::buildDriverDoor()
	{
		std::vector<BasicVertex> doorV;

		float L = length / 2.0f;
		float W = width / 2.0f;
		float G = groundClearance;

		float cabinStart = -L * 0.1f;

		driverDoorHingeLocal = { cabinStart, 0.0f, -W - 0.01f };

		float doorLen = 1.4f;
		float yBottom = G + 0.25f;
		float yTop = G + 1.15f;

		glm::vec3 doorColor = bodyColor * 0.85f;
		glm::vec3 doorNormal = { 0, 0, -1 };
		doorV.push_back({ {0.0f,      yBottom, 0.0f}, doorColor, doorNormal });
		doorV.push_back({ {-doorLen,  yBottom, 0.0f}, doorColor, doorNormal });
		doorV.push_back({ {-doorLen,  yTop,    0.0f}, doorColor, doorNormal });

		doorV.push_back({ {0.0f,      yBottom, 0.0f}, doorColor, doorNormal });
		doorV.push_back({ {-doorLen,  yTop,    0.0f}, doorColor, doorNormal });
		doorV.push_back({ {0.0f,      yTop,    0.0f}, doorColor, doorNormal });

		driverDoor = BasicShape(doorV);

		driverDoorAngle = 0.0f;
		driverDoorTargetAngle = 0.0f;
	}

	void Car::setDriverDoorOpen(bool open)
	{
		driverDoorTargetAngle = open ? driverDoorOpenAngle : 0.0f;
	}

	void Car::setThrottle(float t)
	{
		throttle = clampf(t, -1.0f, 1.0f);
	}

	void Car::setSteer(float s)
	{
		steer = clampf(s, -1.0f, 1.0f);
	}

	void Car::update(float dt)
	{
		float doorSpeed = 180.0f;
		float d = driverDoorTargetAngle - driverDoorAngle;
		float step = doorSpeed * dt;

		if (std::abs(d) <= 0.5f) driverDoorAngle = driverDoorTargetAngle;
		else driverDoorAngle += (d > 0 ? std::min(step, d) : -std::min(step, -d));

		if (std::abs(throttle) > 0.01f)
		{
			speed += throttle * acceleration * dt;
			speed = clampf(speed, -maxReverseSpeed, maxForwardSpeed);
		}
		else
		{
			if (speed > 0) speed = std::max(0.0f, speed - friction * dt);
			if (speed < 0) speed = std::min(0.0f, speed + friction * dt);
		}

		float speedAbs = std::abs(speed);
		if (speedAbs > 0.05f)
		{
			float factor = std::min(1.0f, speedAbs / maxForwardSpeed);
			float dir = (speed >= 0.0f) ? 1.0f : -1.0f;
			rotation += steer * turnRate * dt * factor * dir;

			if (rotation > 360.0f) rotation -= 360.0f;
			if (rotation < 0.0f) rotation += 360.0f;
		}

		float rad = glm::radians(rotation);
		glm::vec3 forward = { (float)std::cos(rad), 0.0f, (float)std::sin(rad) };

		position += forward * speed * dt;

		position.y = baseY;
	}

	void Car::render(const glm::mat4& viewProj)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(bodyRot), glm::vec3(0.0f, 1.0f, 0.0f));
		if (useBodyShape) {
			glm::mat4 correctedModel = model;

			correctedModel = glm::scale(correctedModel, glm::vec3(bodyScale, bodyScale, bodyScale));

			correctedModel = glm::translate(correctedModel, glm::vec3(0.0f, 0.5f, 0.0f));

			bodyShape.render(correctedModel, viewProj);
			return;
		}

		bodyMain.render(model, viewProj);
		bodyHood.render(model, viewProj);
		bodyTrunk.render(model, viewProj);
		bodyRoof.render(model, viewProj);
		bumpers.render(model, viewProj);

		wheels.render(model, viewProj);
		wheelRims.render(model, viewProj);

		headlights.render(model, viewProj);
		taillights.render(model, viewProj);
		indicators.render(model, viewProj);

		doorHandles.render(model, viewProj);
		grille.render(model, viewProj);
		mirrors.render(model, viewProj);

		floorMat.render(model, viewProj);
		seats.render(model, viewProj);
		dashboard.render(model, viewProj);
		steeringWheel.render(model, viewProj);
		gearShift.render(model, viewProj);
		interiorPanels.render(model, viewProj);

		glm::mat4 doorModel = model;
		doorModel = glm::translate(doorModel, driverDoorHingeLocal);
		doorModel = glm::rotate(doorModel, glm::radians(driverDoorAngle), glm::vec3(0, 1, 0));
		driverDoor.render(doorModel, viewProj);

		windowFront.render(model, viewProj, 0.4f);
		windowRear.render(model, viewProj, 0.4f);
		windowsLeft.render(model, viewProj, 0.4f);
		windowsRight.render(model, viewProj, 0.4f);



	}

	glm::vec3 Car::getDriverSeatPosition() const
	{
		float driverLocalX = -0.35f;
		float driverLocalY = groundClearance + 1.2f;
		float driverLocalZ = -width * 0.25f;

		float rad = glm::radians(rotation);
		float cosR = cos(rad);
		float sinR = sin(rad);

		float worldX = position.x + driverLocalX * cosR - driverLocalZ * sinR;
		float worldY = position.y + driverLocalY;
		float worldZ = position.z + driverLocalX * sinR + driverLocalZ * cosR;

		return glm::vec3(worldX, worldY, worldZ);
	}

	glm::vec3 Car::getDriverViewDirection() const
	{
		float rad = glm::radians(rotation);
		return glm::vec3(cos(rad), 0.0f, sin(rad));
	}

	bool Car::isNearDriverDoor(glm::vec3 playerPos) const
	{
		float dist = sqrt(
			pow(playerPos.x - position.x, 2) +
			pow(playerPos.z - position.z, 2)
		);
		return dist < 7.0f;
	}

	void Car::getCollisionBounds(float& minX, float& maxX, float& minZ, float& maxZ) const
	{
		float halfL = length / 2.0f + 0.1f;
		float halfW = width / 2.0f + 0.1f;

		minX = position.x - halfL;
		maxX = position.x + halfL;
		minZ = position.z - halfW;
		maxZ = position.z + halfW;
	}
}