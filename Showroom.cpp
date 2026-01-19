#include "Showroom.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "SportsCar.hpp"
#include "SUVCar.hpp" 
extern void audioPlay(const char* file);

namespace Example
{
	void Showroom::init()
	{
		float showroomSize = 70.0f;
		float wallHeight = 15.0f;

		float mainDoorWidth = 10.0f;
		float mainDoorHeight = 5.0f;
		float mainDoorHalfW = mainDoorWidth / 2.0f;

		float windowWidth = 35.0f;
		float windowBottom = 1.5f;
		float windowTop = 12.0f;
		float windowHalfW = windowWidth / 2.0f;

		float PI = 3.14159265f;

		exteriorBounds.minX = -showroomSize;
		exteriorBounds.maxX = showroomSize;
		exteriorBounds.minZ = -showroomSize;
		exteriorBounds.maxZ = showroomSize;
		exteriorBounds.doorMinX = -mainDoorHalfW;
		exteriorBounds.doorMaxX = mainDoorHalfW;
		exteriorBounds.doorZ = showroomSize;

		float S = showroomSize;
		float H = wallHeight;

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

		

		std::vector<BasicVertex> ceilVerts;
		glm::vec3 ceilColor = { 0.12f, 0.12f, 0.15f };
		ceilVerts.push_back({ {-S, H, -S}, ceilColor });
		ceilVerts.push_back({ {S, H, -S}, ceilColor });
		ceilVerts.push_back({ {S, H, S}, ceilColor });
		ceilVerts.push_back({ {-S, H, -S}, ceilColor });
		ceilVerts.push_back({ {S, H, S}, ceilColor });
		ceilVerts.push_back({ {-S, H, S}, ceilColor });
		ceiling = BasicShape(ceilVerts);

		std::vector<BasicVertex> wallVerts;
		glm::vec3 wallColorOut = { 0.25f, 0.25f, 0.28f };
		glm::vec3 wallColorIn = { 0.35f, 0.35f, 0.38f };

		auto addQuad = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 col)
			{
				wallVerts.push_back({ p1, col });
				wallVerts.push_back({ p2, col });
				wallVerts.push_back({ p3, col });
				wallVerts.push_back({ p1, col });
				wallVerts.push_back({ p3, col });
				wallVerts.push_back({ p4, col });
			};

		addQuad({ -S, 0, -S }, { -windowHalfW, 0, -S }, { -windowHalfW, H, -S }, { -S, H, -S }, wallColorIn);
		addQuad({ windowHalfW, 0, -S }, { S, 0, -S }, { S, H, -S }, { windowHalfW, H, -S }, wallColorIn);
		addQuad({ -windowHalfW, 0, -S }, { windowHalfW, 0, -S }, { windowHalfW, windowBottom, -S }, { -windowHalfW, windowBottom, -S }, wallColorIn);
		addQuad({ -windowHalfW, windowTop, -S }, { windowHalfW, windowTop, -S }, { windowHalfW, H, -S }, { -windowHalfW, H, -S }, wallColorIn);

		addQuad({ -S, 0, S }, { -mainDoorHalfW, 0, S }, { -mainDoorHalfW, H, S }, { -S, H, S }, wallColorOut);
		addQuad({ mainDoorHalfW, 0, S }, { S, 0, S }, { S, H, S }, { mainDoorHalfW, H, S }, wallColorOut);
		addQuad({ -mainDoorHalfW, mainDoorHeight, S }, { mainDoorHalfW, mainDoorHeight, S }, { mainDoorHalfW, H, S }, { -mainDoorHalfW, H, S }, wallColorOut);

		addQuad({ -S, 0, -S }, { -S, 0, -windowHalfW }, { -S, H, -windowHalfW }, { -S, H, -S }, wallColorOut);
		addQuad({ -S, 0, windowHalfW }, { -S, 0, S }, { -S, H, S }, { -S, H, windowHalfW }, wallColorOut);
		addQuad({ -S, 0, -windowHalfW }, { -S, 0, windowHalfW }, { -S, windowBottom, windowHalfW }, { -S, windowBottom, -windowHalfW }, wallColorOut);
		addQuad({ -S, windowTop, -windowHalfW }, { -S, windowTop, windowHalfW }, { -S, H, windowHalfW }, { -S, H, -windowHalfW }, wallColorOut);

		addQuad({ S, 0, -windowHalfW }, { S, 0, -S }, { S, H, -S }, { S, H, -windowHalfW }, wallColorOut);
		addQuad({ S, 0, S }, { S, 0, windowHalfW }, { S, H, windowHalfW }, { S, H, S }, wallColorOut);
		addQuad({ S, 0, windowHalfW }, { S, 0, -windowHalfW }, { S, windowBottom, -windowHalfW }, { S, windowBottom, windowHalfW }, wallColorOut);
		addQuad({ S, windowTop, windowHalfW }, { S, windowTop, -windowHalfW }, { S, H, -windowHalfW }, { S, H, windowHalfW }, wallColorOut);

		exteriorWalls = BasicShape(wallVerts);

		std::vector<BasicVertex> glassVerts;
		glm::vec3 glassColor = { 0.6f, 0.8f, 0.9f };

		auto addGlassQuad = [&](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 col)
			{
				glassVerts.push_back({ p1, col });
				glassVerts.push_back({ p2, col });
				glassVerts.push_back({ p3, col });
				glassVerts.push_back({ p1, col });
				glassVerts.push_back({ p3, col });
				glassVerts.push_back({ p4, col });
			};

		addGlassQuad({ -windowHalfW, windowBottom, -S + 0.05f }, { windowHalfW, windowBottom, -S + 0.05f },
			{ windowHalfW, windowTop, -S + 0.05f }, { -windowHalfW, windowTop, -S + 0.05f }, glassColor);
		addGlassQuad({ -S + 0.05f, windowBottom, -windowHalfW }, { -S + 0.05f, windowBottom, windowHalfW },
			{ -S + 0.05f, windowTop, windowHalfW }, { -S + 0.05f, windowTop, -windowHalfW }, glassColor);
		addGlassQuad({ S - 0.05f, windowBottom, windowHalfW }, { S - 0.05f, windowBottom, -windowHalfW },
			{ S - 0.05f, windowTop, -windowHalfW }, { S - 0.05f, windowTop, windowHalfW }, glassColor);

		windowGlass = BasicShape(glassVerts);

		std::vector<BasicVertex> frameVerts;
		glm::vec3 frameColor = { 0.1f, 0.1f, 0.12f };
		float frameW = 0.3f;

		frameVerts.push_back({ {-windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW + frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW - frameW, windowTop, -S + 0.1f}, frameColor });

		frameVerts.push_back({ {windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW + frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW - frameW, windowBottom, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW + frameW, windowTop, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW - frameW, windowTop, -S + 0.1f}, frameColor });

		frameVerts.push_back({ {-windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW, windowTop - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW, windowTop + frameW, -S + 0.1f}, frameColor });

		frameVerts.push_back({ {-windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW, windowBottom - frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });
		frameVerts.push_back({ {-windowHalfW, windowBottom + frameW, -S + 0.1f}, frameColor });

		windowFrames = BasicShape(frameVerts);

		std::vector<BasicVertex> doorFrameVerts;
		glm::vec3 doorFrameCol = { 0.08f, 0.08f, 0.1f };
		float dfW = 0.4f;

		doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW - dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });

		doorFrameVerts.push_back({ {mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW + dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW - dfW, 0, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW + dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW - dfW, mainDoorHeight, S - 0.1f}, doorFrameCol });

		doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight - dfW, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });
		doorFrameVerts.push_back({ {-mainDoorHalfW, mainDoorHeight + dfW, S - 0.1f}, doorFrameCol });

		mainDoorFrame = BasicShape(doorFrameVerts);

		std::vector<BasicVertex> colVerts;
		glm::vec3 colMain = { 0.35f, 0.35f, 0.40f };
		glm::vec3 colDark = { 0.25f, 0.25f, 0.30f };
		glm::vec3 colBase = { 0.45f, 0.45f, 0.50f };
		glm::vec3 colAccent = { 0.55f, 0.55f, 0.60f };

		float cw = 1.0f;
		float baseW = 1.4f;
		float baseH = 0.5f;
		float capH = 0.4f;

		float columnPositions[4][2] = {
			{20.0f, 20.0f}, {-20.0f, 20.0f},
			{20.0f, -20.0f}, {-20.0f, -20.0f}
		};

		columnBounds.clear();

		for (int i = 0; i < 4; i++)
		{
			float x = columnPositions[i][0];
			float z = columnPositions[i][1];

			colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });

			colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });

			colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x - baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });

			colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x + baseW, 0, z - baseW}, colBase });
			colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, 0, z + baseW}, colBase });
			colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });

			colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, baseH, z - baseW}, colAccent });
			colVerts.push_back({ {x + baseW, baseH, z + baseW}, colAccent });
			colVerts.push_back({ {x - baseW, baseH, z + baseW}, colAccent });

			float bodyBottom = baseH;
			float bodyTop = H - capH;

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

			float capBottom = H - capH;
			float capTop = H;

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

		rooms.push_back(std::make_unique<Room>());
		rooms.back()->create("Luxury", { 45, 0, -45 }, 18.0f, { 0.15f, 0.15f, 0.15f }, true);

		rooms.push_back(std::make_unique<Room>());
		rooms.back()->create("Electric", { -45, 0, -45 }, 18.0f, { 0.0f, 0.2f, 0.5f }, true);

		rooms.push_back(std::make_unique<Room>());
		rooms.back()->create("Sports", { 45, 0, 45 }, 18.0f, { 0.6f, 0.0f, 0.0f }, false);

		rooms.push_back(std::make_unique<Room>());
		rooms.back()->create("Family", { -45, 0, 45 }, 18.0f, { 0.0f, 0.5f, 0.0f }, false);



		rooms[0]->addCarToPodium(1, { 0.7f, 0.1f, 0.1f }, { 0.2f, 0.15f, 0.1f });




		glm::vec3 sportsRoomCenter = rooms[2]->centerOffset;
		sportsCarInRoom.create(
			glm::vec3(sportsRoomCenter.x, 0.55f, sportsRoomCenter.z + 5.25f),
			0.0f,
			glm::vec3(0.95f, 0.75f, 0.1f));

		glm::vec3 familyRoomCenter = rooms[3]->centerOffset;
		suvCarInRoom.create(
			glm::vec3(familyRoomCenter.x, 0.65f, familyRoomCenter.z + 5.25f),
			0.0f,
			glm::vec3(0.15f, 0.55f, 0.2f));

		glm::vec3 electricRoomCenter = rooms[1]->centerOffset;
		electricCarInRoom.create(
			glm::vec3(electricRoomCenter.x, 0.55f, electricRoomCenter.z - 5.25f),
			180.0f,
			glm::vec3(0.1f, 0.35f, 0.75f));

		Example::Car* modelCar = new Example::Car();
		Example::loadModelToShape(modelCar->bodyShape, "assets/models/Porsche.glb", { 1.0f, 1.0f, 1.0f });
		modelCar->useBodyShape = true;           modelCar->bodyScale = 1.0f;          modelCar->bodyRot = 180.0f;
		rooms[2]->addExistingCarToPodium(0, modelCar);

		Example::Car* modelCar1 = new Example::Car();
		Example::loadModelToShape(modelCar1->bodyShape, "assets/models/Dodge.glb", { 1.0f, 1.0f, 1.0f });
		modelCar1->useBodyShape = true;                   modelCar1->bodyScale = 1.6f;          modelCar1->bodyRot = 180.0f;

		rooms[2]->addExistingCarToPodium(2, modelCar1);

		Example::Car* modelCar2 = new Example::Car();
		Example::loadModelToShape(modelCar2->bodyShape, "assets/models/Cadillac.glb", { 1.0f, 1.0f, 1.0f });
		modelCar2->useBodyShape = true;                   modelCar2->bodyScale = 1.0f;          modelCar2->bodyRot = 0.0f;

		rooms[0]->addExistingCarToPodium(0, modelCar2);
		Example::Car* modelCar7 = new Example::Car();
		Example::loadModelToShape(modelCar7->bodyShape, "assets/models/luxury.glb", { 1.0f, 1.0f, 1.0f });
		modelCar7->useBodyShape = true;                   modelCar7->bodyScale = 0.25f;          modelCar7->bodyRot = 180.0f;

		rooms[0]->addExistingCarToPodium(2, modelCar7);
		Example::Car* modelCar3 = new Example::Car();
		Example::loadModelToShape(modelCar3->bodyShape, "assets/models/classic.glb", { 1.0f, 1.0f, 1.0f });
		modelCar3->useBodyShape = true;                   modelCar3->bodyScale = 1.0f;          modelCar3->bodyRot = 180.0f;

		rooms[3]->addExistingCarToPodium(2, modelCar3);
		Example::Car* modelCar4 = new Example::Car();
		Example::loadModelToShape(modelCar4->bodyShape, "assets/models/ford_classic.glb", { 1.0f, 1.0f, 1.0f });
		modelCar4->useBodyShape = true;                   modelCar4->bodyScale = 1.3f;          modelCar4->bodyRot = -90.0f;

		rooms[3]->addExistingCarToPodium(0, modelCar4);
		Example::Car* modelCar5 = new Example::Car();
		Example::loadModelToShape(modelCar5->bodyShape, "assets/models/electric.glb", { 1.0f, 1.0f, 1.0f });
		modelCar5->useBodyShape = true;                   modelCar5->bodyScale = 0.02f;          modelCar5->bodyRot = 0.0f;

		rooms[1]->addExistingCarToPodium(0, modelCar5);
		Example::Car* modelCar6 = new Example::Car();
		Example::loadModelToShape(modelCar6->bodyShape, "assets/models/electric2.glb", { 1.0f, 1.0f, 1.0f });
		modelCar6->useBodyShape = true;                   modelCar6->bodyScale = 0.02f;          modelCar6->bodyRot = 0.0f;

		rooms[1]->addExistingCarToPodium(2, modelCar6);
		std::cout << " Cars added to showroom!" << std::endl;
		std::cout << " Press E near a car to enter/exit" << std::endl;


		//street

		std::vector<BasicVertex> roadV;
		glm::vec3 asphaltCol = { 0.15f, 0.15f, 0.15f };

		float roadW = 6.0f;
		float roadLongNS = 150.0f;
		float roadLongEW = 80.0f;
		float intersectSize = roadW;
		roadV.push_back({ {-roadW, 0.02f, intersectSize}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, intersectSize}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, roadLongNS}, asphaltCol });
		roadV.push_back({ {-roadW, 0.02f, intersectSize}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, roadLongNS}, asphaltCol });
		roadV.push_back({ {-roadW, 0.02f, roadLongNS}, asphaltCol });

		roadV.push_back({ {-roadW, 0.02f, -roadLongNS}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, -roadLongNS}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, -intersectSize}, asphaltCol });
		roadV.push_back({ {-roadW, 0.02f, -roadLongNS}, asphaltCol });
		roadV.push_back({ {roadW, 0.02f, -intersectSize}, asphaltCol });
		roadV.push_back({ {-roadW, 0.02f, -intersectSize}, asphaltCol });

		roadV.push_back({ {intersectSize, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {roadLongEW, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {roadLongEW, 0.02f, roadW}, asphaltCol });
		roadV.push_back({ {intersectSize, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {roadLongEW, 0.02f, roadW}, asphaltCol });
		roadV.push_back({ {intersectSize, 0.02f, roadW}, asphaltCol });

		roadV.push_back({ {-roadLongEW, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {-intersectSize, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {-intersectSize, 0.02f, roadW}, asphaltCol });
		roadV.push_back({ {-roadLongEW, 0.02f, -roadW}, asphaltCol });
		roadV.push_back({ {-intersectSize, 0.02f, roadW}, asphaltCol });
		roadV.push_back({ {-roadLongEW, 0.02f, roadW}, asphaltCol });

		roadV.push_back({ {-intersectSize, 0.02f, -intersectSize}, asphaltCol });
		roadV.push_back({ {intersectSize, 0.02f, -intersectSize}, asphaltCol });
		roadV.push_back({ {intersectSize, 0.02f, intersectSize}, asphaltCol });
		roadV.push_back({ {-intersectSize, 0.02f, -intersectSize}, asphaltCol });
		roadV.push_back({ {intersectSize, 0.02f, intersectSize}, asphaltCol });
		roadV.push_back({ {-intersectSize, 0.02f, intersectSize}, asphaltCol });

		streetAsphalt = BasicShape(roadV);

		//الدوار

		std::vector<BasicVertex> roundaboutV;
		glm::vec3 greenCol = { 0.15f, 0.45f, 0.15f };
		glm::vec3 darkGreen = { 0.1f, 0.35f, 0.1f };
		glm::vec3 curbCol = { 0.5f, 0.5f, 0.52f };

		float centerRadius = 3.0f;
		float yLevel = 0.05f;
		int segments = 32;

		for (int i = 0; i < segments; i++)
		{
			float angle1 = (2.0f * PI * i) / segments;
			float angle2 = (2.0f * PI * (i + 1)) / segments;

			glm::vec3 p1 = { 0.0f, yLevel + 0.15f, 0.0f };
			glm::vec3 p2 = { centerRadius * cos(angle1), yLevel + 0.15f, centerRadius * sin(angle1) };
			glm::vec3 p3 = { centerRadius * cos(angle2), yLevel + 0.15f, centerRadius * sin(angle2) };

			glm::vec3 col = (i % 2 == 0) ? greenCol : darkGreen;
			roundaboutV.push_back({ p1, greenCol });
			roundaboutV.push_back({ p2, col });
			roundaboutV.push_back({ p3, col });
		}

		float curbWidth = 0.4f;
		for (int i = 0; i < segments; i++)
		{
			float angle1 = (2.0f * PI * i) / segments;
			float angle2 = (2.0f * PI * (i + 1)) / segments;

			float r1 = centerRadius;
			float r2 = centerRadius + curbWidth;

			glm::vec3 in1 = { r1 * cos(angle1), yLevel + 0.15f, r1 * sin(angle1) };
			glm::vec3 in2 = { r1 * cos(angle2), yLevel + 0.15f, r1 * sin(angle2) };
			glm::vec3 out1 = { r2 * cos(angle1), yLevel + 0.10f, r2 * sin(angle1) };
			glm::vec3 out2 = { r2 * cos(angle2), yLevel + 0.10f, r2 * sin(angle2) };

			roundaboutV.push_back({ in1, curbCol });
			roundaboutV.push_back({ out1, curbCol * 0.8f });
			roundaboutV.push_back({ out2, curbCol * 0.8f });
			roundaboutV.push_back({ in1, curbCol });
			roundaboutV.push_back({ out2, curbCol * 0.8f });
			roundaboutV.push_back({ in2, curbCol });

			glm::vec3 outBottom1 = { r2 * cos(angle1), yLevel, r2 * sin(angle1) };
			glm::vec3 outBottom2 = { r2 * cos(angle2), yLevel, r2 * sin(angle2) };

			roundaboutV.push_back({ outBottom1, curbCol * 0.6f });
			roundaboutV.push_back({ out1, curbCol * 0.8f });
			roundaboutV.push_back({ out2, curbCol * 0.8f });
			roundaboutV.push_back({ outBottom1, curbCol * 0.6f });
			roundaboutV.push_back({ out2, curbCol * 0.8f });
			roundaboutV.push_back({ outBottom2, curbCol * 0.6f });
		}

		roundabout = BasicShape(roundaboutV);


		//رصيف
		std::vector<BasicVertex> sideV;
		glm::vec3 lightGrey = { 0.7f, 0.7f, 0.7f };
		glm::vec3 darkGrey = { 0.3f, 0.3f, 0.3f };
		glm::vec3 yellow = { 0.9f, 0.8f, 0.0f };

		float sW = 1.5f;
		float sidewalkWidth = sW * 2;         
		float tileL = 2.0f;
		float yTop = 0.25f;
		float yBottom = 0.02f;

		float sidewalkEdge = roadW + sidewalkWidth;    
		float cornerStart = intersectSize;      
		float cornerEnd = intersectSize + sidewalkWidth;
		auto addSidewalkRect = [&](float x1, float x2, float z1, float z2,
			bool curbLeft, bool curbRight, bool curbFront, bool curbBack)
			{
				glm::vec3 col = lightGrey;

				sideV.push_back({ {x1, yTop, z1}, col });
				sideV.push_back({ {x2, yTop, z1}, col });
				sideV.push_back({ {x2, yTop, z2}, col });
				sideV.push_back({ {x1, yTop, z1}, col });
				sideV.push_back({ {x2, yTop, z2}, col });
				sideV.push_back({ {x1, yTop, z2}, col });

				if (curbLeft)
				{
					sideV.push_back({ {x1, yBottom, z1}, yellow });
					sideV.push_back({ {x1, yTop, z1}, darkGrey });
					sideV.push_back({ {x1, yTop, z2}, darkGrey });
					sideV.push_back({ {x1, yBottom, z1}, yellow });
					sideV.push_back({ {x1, yTop, z2}, darkGrey });
					sideV.push_back({ {x1, yBottom, z2}, yellow });
				}

				if (curbRight)
				{
					sideV.push_back({ {x2, yBottom, z1}, yellow });
					sideV.push_back({ {x2, yTop, z1}, darkGrey });
					sideV.push_back({ {x2, yTop, z2}, darkGrey });
					sideV.push_back({ {x2, yBottom, z1}, yellow });
					sideV.push_back({ {x2, yTop, z2}, darkGrey });
					sideV.push_back({ {x2, yBottom, z2}, yellow });
				}

				if (curbFront)
				{
					sideV.push_back({ {x1, yBottom, z2}, yellow });
					sideV.push_back({ {x1, yTop, z2}, darkGrey });
					sideV.push_back({ {x2, yTop, z2}, darkGrey });
					sideV.push_back({ {x1, yBottom, z2}, yellow });
					sideV.push_back({ {x2, yTop, z2}, darkGrey });
					sideV.push_back({ {x2, yBottom, z2}, yellow });
				}

				if (curbBack)
				{
					sideV.push_back({ {x1, yBottom, z1}, yellow });
					sideV.push_back({ {x1, yTop, z1}, darkGrey });
					sideV.push_back({ {x2, yTop, z1}, darkGrey });
					sideV.push_back({ {x1, yBottom, z1}, yellow });
					sideV.push_back({ {x2, yTop, z1}, darkGrey });
					sideV.push_back({ {x2, yBottom, z1}, yellow });
				}
			};

		auto addVerticalSidewalk = [&](float xPos, float zStart, float zEnd)
			{
				float xMin = xPos - sW;
				float xMax = xPos + sW;
				bool isRight = (xPos > 0);

				for (float z = zStart; z < zEnd; z += tileL)
				{
					glm::vec3 currentCol = ((int)((z - zStart) / tileL) % 2 == 0) ? lightGrey : lightGrey * 0.92f;
					float zMin = z;
					float zMax = std::min(z + tileL - 0.1f, zEnd);

					sideV.push_back({ {xMin, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMax}, currentCol });
					sideV.push_back({ {xMin, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMax}, currentCol });
					sideV.push_back({ {xMin, yTop, zMax}, currentCol });

					float faceX = isRight ? xMin : xMax;
					sideV.push_back({ {faceX, yBottom, zMin}, yellow });
					sideV.push_back({ {faceX, yTop, zMin}, darkGrey });
					sideV.push_back({ {faceX, yTop, zMax}, darkGrey });
					sideV.push_back({ {faceX, yBottom, zMin}, yellow });
					sideV.push_back({ {faceX, yTop, zMax}, darkGrey });
					sideV.push_back({ {faceX, yBottom, zMax}, yellow });
				}
			};

		auto addHorizontalSidewalk = [&](float zPos, float xStart, float xEnd)
			{
				float zMin = zPos - sW;
				float zMax = zPos + sW;
				bool isFront = (zPos > 0);

				for (float x = xStart; x < xEnd; x += tileL)
				{
					glm::vec3 currentCol = ((int)((x - xStart) / tileL) % 2 == 0) ? lightGrey : lightGrey * 0.92f;
					float xMin = x;
					float xMax = std::min(x + tileL - 0.1f, xEnd);

					sideV.push_back({ {xMin, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMax}, currentCol });
					sideV.push_back({ {xMin, yTop, zMin}, currentCol });
					sideV.push_back({ {xMax, yTop, zMax}, currentCol });
					sideV.push_back({ {xMin, yTop, zMax}, currentCol });

					float faceZ = isFront ? zMin : zMax;
					sideV.push_back({ {xMin, yBottom, faceZ}, yellow });
					sideV.push_back({ {xMin, yTop, faceZ}, darkGrey });
					sideV.push_back({ {xMax, yTop, faceZ}, darkGrey });
					sideV.push_back({ {xMin, yBottom, faceZ}, yellow });
					sideV.push_back({ {xMax, yTop, faceZ}, darkGrey });
					sideV.push_back({ {xMax, yBottom, faceZ}, yellow });
				}
			};


		addVerticalSidewalk(roadW + sW, cornerEnd, roadLongNS);      
		addVerticalSidewalk(roadW + sW, -roadLongNS, -cornerEnd);      
		addVerticalSidewalk(-(roadW + sW), cornerEnd, roadLongNS);        
		addVerticalSidewalk(-(roadW + sW), -roadLongNS, -cornerEnd);
		addHorizontalSidewalk(roadW + sW, cornerEnd, roadLongEW);         
		addHorizontalSidewalk(roadW + sW, -roadLongEW, -cornerEnd);          
		addHorizontalSidewalk(-(roadW + sW), cornerEnd, roadLongEW);     
		addHorizontalSidewalk(-(roadW + sW), -roadLongEW, -cornerEnd);

		addSidewalkRect(cornerStart, cornerEnd, cornerStart, cornerEnd, true, false, false, true);

		addSidewalkRect(-cornerEnd, -cornerStart, cornerStart, cornerEnd, false, true, false, true);

		addSidewalkRect(cornerStart, cornerEnd, -cornerEnd, -cornerStart, true, false, true, false);

		addSidewalkRect(-cornerEnd, -cornerStart, -cornerEnd, -cornerStart, false, true, true, false);

		sidewalk = BasicShape(sideV);

		//street lines
		std::vector<BasicVertex> lineV;
		glm::vec3 lineCol = { 1.0f, 1.0f, 1.0f };
		float lW = 0.12f;
		float lL = 3.5f;
		float lineSpacing = 8.0f;
		float lineStart = intersectSize + 2.0f;

		for (float z = lineStart; z < roadLongNS - lL; z += lineSpacing)
		{
			lineV.push_back({ {-lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
			lineV.push_back({ {-lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
			lineV.push_back({ {-lW, 0.08f, z + lL}, lineCol });
		}

		for (float z = -roadLongNS + 3.0f; z < -lineStart; z += lineSpacing)
		{
			lineV.push_back({ {-lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
			lineV.push_back({ {-lW, 0.08f, z}, lineCol });
			lineV.push_back({ {lW, 0.08f, z + lL}, lineCol });
			lineV.push_back({ {-lW, 0.08f, z + lL}, lineCol });
		}

		for (float x = lineStart; x < roadLongEW - lL; x += lineSpacing)
		{
			lineV.push_back({ {x, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, lW}, lineCol });
			lineV.push_back({ {x, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, lW}, lineCol });
			lineV.push_back({ {x, 0.08f, lW}, lineCol });
		}

		for (float x = -roadLongEW + 3.0f; x < -lineStart; x += lineSpacing)
		{
			lineV.push_back({ {x, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, lW}, lineCol });
			lineV.push_back({ {x, 0.08f, -lW}, lineCol });
			lineV.push_back({ {x + lL, 0.08f, lW}, lineCol });
			lineV.push_back({ {x, 0.08f, lW}, lineCol });
		}

		streetLines = BasicShape(lineV);


		//عواميد الضو

		lighting.ambientColor = { 0.1f, 0.1f, 0.15f };
		lighting.ambientIntensity = 0.25f;

		lighting.directionalLight = DirectionalLight(
			{ 0.2f, -0.8f, 0.3f }, { 1.0f, 0.95f, 0.85f }, 0.3f);


		lighting.addPointLight(PointLight({ 0, 14, 0 }, { 1.0f, 1.0f, 0.95f }, 1.5f, 40.0f));
		lighting.addPointLight(PointLight({ 25, 14, 25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
		lighting.addPointLight(PointLight({ -25, 14, 25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
		lighting.addPointLight(PointLight({ 25, 14, -25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));
		lighting.addPointLight(PointLight({ -25, 14, -25 }, { 1.0f, 1.0f, 0.95f }, 1.0f, 30.0f));


		lighting.addPointLight(PointLight({ 45, 6.5f, -45 }, { 1.0f, 0.9f, 0.7f }, 1.2f, 25.0f));

		lighting.addPointLight(PointLight({ -45, 6.5f, -45 }, { 0.5f, 0.7f, 1.0f }, 1.2f, 25.0f));

		lighting.addPointLight(PointLight({ 45, 6.5f, 45 }, { 1.0f, 0.6f, 0.5f }, 1.2f, 25.0f));

		lighting.addPointLight(PointLight({ -45, 6.5f, 45 }, { 0.8f, 1.0f, 0.7f }, 1.2f, 25.0f));


		lighting.addSpotLight(SpotLight(
			{ 45, 6.0f, -50.25f }, { 0, -1, 0 }, { 1.0f, 0.95f, 0.8f }, 2.0f, 30.0f, 45.0f));

		lighting.addSpotLight(SpotLight(
			{ -45, 6.0f, -50.25f },
			{ 0, -1, 0 },
			{ 0.6f, 0.8f, 1.0f }, 2.0f,
			30.0f, 45.0f
		));

		lighting.addSpotLight(SpotLight(
			{ 45, 6.0f, 39.75f },
			{ 0, -1, 0 },
			{ 1.0f, 0.9f, 0.8f },
			2.0f,
			30.0f, 45.0f
		));

		lighting.addSpotLight(SpotLight(
			{ -45, 6.0f, 39.75f },
			{ 0, -1, 0 },
			{ 0.9f, 1.0f, 0.85f }, 2.0f,
			30.0f, 45.0f
		));

		std::cout << " Lighting system initialized!" << std::endl;
		std::cout << "   Point Lights: " << lighting.pointLights.size() << std::endl;
		std::cout << "   Spot Lights: " << lighting.spotLights.size() << std::endl;
		std::cout << "   Press L to toggle lights" << std::endl;

		std::cout << " Showroom initialized!" << std::endl;
		std::cout << "   Road width: " << roadW * 2 << std::endl;
		std::cout << "   Intersection: " << intersectSize * 2 << " x " << intersectSize * 2 << std::endl;
		std::cout << "   Sidewalk width: " << sidewalkWidth << std::endl;
		std::cout << "   Corner: from " << cornerStart << " to " << cornerEnd << std::endl;


		// spot lights on the side walks 

		std::vector<BasicVertex> lampV;
		glm::vec3 poleCol = { 0.2f, 0.2f, 0.22f };
		glm::vec3 poleDark = { 0.12f, 0.12f, 0.14f };
		glm::vec3 poleLight = { 0.3f, 0.3f, 0.32f };
		glm::vec3 lampCol = { 1.0f, 0.95f, 0.75f };
		glm::vec3 lampGlow = { 1.0f, 0.85f, 0.5f };
		glm::vec3 armCol = { 0.18f, 0.18f, 0.2f };

		float lampHeight = 6.0f;
		float poleWidth = 0.1f;
		float armLen = 1.8f;
		float lampSize = 0.35f;
		float lampSpacing = 18.0f;

		auto addLampPost = [&](float x, float z, float armDirX, float armDirZ)
			{
				float pw = poleWidth;
				float bw = 0.22f;
				float bh = 0.25f;

				//draw the base of spot light

				lampV.push_back({ {x - bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x + bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x + bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x + bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, bh, z + bw}, poleLight });

				lampV.push_back({ {x + bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x - bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x - bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x - bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, bh, z - bw}, poleLight });

				lampV.push_back({ {x - bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x - bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x - bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x - bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, bh, z - bw}, poleLight });

				lampV.push_back({ {x + bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x + bw, 0, z - bw}, poleDark });
				lampV.push_back({ {x + bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, 0, z + bw}, poleDark });
				lampV.push_back({ {x + bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, bh, z + bw}, poleLight });

				lampV.push_back({ {x - bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, bh, z - bw}, poleLight });
				lampV.push_back({ {x + bw, bh, z + bw}, poleLight });
				lampV.push_back({ {x - bw, bh, z + bw}, poleLight });

				float bodyBottom = bh;
				float bodyTop = lampHeight;

				//the column

				lampV.push_back({ {x - pw, bodyBottom, z + pw}, poleCol });
				lampV.push_back({ {x + pw, bodyBottom, z + pw}, poleCol });
				lampV.push_back({ {x + pw, bodyTop, z + pw}, poleCol });
				lampV.push_back({ {x - pw, bodyBottom, z + pw}, poleCol });
				lampV.push_back({ {x + pw, bodyTop, z + pw}, poleCol });
				lampV.push_back({ {x - pw, bodyTop, z + pw}, poleCol });

				lampV.push_back({ {x + pw, bodyBottom, z - pw}, poleCol });
				lampV.push_back({ {x - pw, bodyBottom, z - pw}, poleCol });
				lampV.push_back({ {x - pw, bodyTop, z - pw}, poleCol });
				lampV.push_back({ {x + pw, bodyBottom, z - pw}, poleCol });
				lampV.push_back({ {x - pw, bodyTop, z - pw}, poleCol });
				lampV.push_back({ {x + pw, bodyTop, z - pw}, poleCol });

				lampV.push_back({ {x - pw, bodyBottom, z - pw}, poleDark });
				lampV.push_back({ {x - pw, bodyBottom, z + pw}, poleDark });
				lampV.push_back({ {x - pw, bodyTop, z + pw}, poleDark });
				lampV.push_back({ {x - pw, bodyBottom, z - pw}, poleDark });
				lampV.push_back({ {x - pw, bodyTop, z + pw}, poleDark });
				lampV.push_back({ {x - pw, bodyTop, z - pw}, poleDark });

				lampV.push_back({ {x + pw, bodyBottom, z + pw}, poleLight });
				lampV.push_back({ {x + pw, bodyBottom, z - pw}, poleLight });
				lampV.push_back({ {x + pw, bodyTop, z - pw}, poleLight });
				lampV.push_back({ {x + pw, bodyBottom, z + pw}, poleLight });
				lampV.push_back({ {x + pw, bodyTop, z - pw}, poleLight });
				lampV.push_back({ {x + pw, bodyTop, z + pw}, poleLight });


				float armY = lampHeight - 0.05f;
				float armH = 0.08f;
				float armW = 0.06f;
				float armEndX = x + armDirX * armLen;
				float armEndZ = z + armDirZ * armLen;
				float armEndY = armY - 0.4f;

				lampV.push_back({ {x - armW, armY - armH, z - armW}, armCol });
				lampV.push_back({ {x + armW, armY - armH, z + armW}, armCol });
				lampV.push_back({ {armEndX + armW, armEndY - armH, armEndZ + armW}, armCol });
				lampV.push_back({ {x - armW, armY - armH, z - armW}, armCol });
				lampV.push_back({ {armEndX + armW, armEndY - armH, armEndZ + armW}, armCol });
				lampV.push_back({ {armEndX - armW, armEndY - armH, armEndZ - armW}, armCol });

				lampV.push_back({ {x - armW, armY + armH, z - armW}, armCol });
				lampV.push_back({ {x + armW, armY + armH, z + armW}, armCol });
				lampV.push_back({ {armEndX + armW, armEndY + armH, armEndZ + armW}, armCol });
				lampV.push_back({ {x - armW, armY + armH, z - armW}, armCol });
				lampV.push_back({ {armEndX + armW, armEndY + armH, armEndZ + armW}, armCol });
				lampV.push_back({ {armEndX - armW, armEndY + armH, armEndZ - armW}, armCol });

				float lx = armEndX;
				float lz = armEndZ;
				float ly = armEndY - lampSize - 0.1f;
				float ls = lampSize;

				lampV.push_back({ {lx - ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx + ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz + ls}, lampGlow });

				lampV.push_back({ {lx + ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx - ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz - ls}, lampGlow });

				lampV.push_back({ {lx - ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx - ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz - ls}, lampGlow });

				lampV.push_back({ {lx + ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx + ls, ly, lz - ls}, lampCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly, lz + ls}, lampCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz + ls}, lampGlow });

				lampV.push_back({ {lx - ls, ly, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly, lz - ls}, lampGlow });
				lampV.push_back({ {lx + ls, ly, lz + ls}, lampGlow });
				lampV.push_back({ {lx - ls, ly, lz + ls}, lampGlow });

				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz - ls}, armCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz - ls}, armCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz + ls}, armCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz - ls}, armCol });
				lampV.push_back({ {lx + ls, ly + ls * 1.5f, lz + ls}, armCol });
				lampV.push_back({ {lx - ls, ly + ls * 1.5f, lz + ls}, armCol });
			};

		float sidewalkPosR = roadW + sW;
		float sidewalkPosL = -(roadW + sW);

		for (float z = cornerEnd + 8; z < roadLongNS - 10; z += lampSpacing)
			addLampPost(sidewalkPosR, z, -1, 0);

		for (float z = cornerEnd + 8; z < roadLongNS - 10; z += lampSpacing)
			addLampPost(sidewalkPosL, z, 1, 0);

		for (float z = -roadLongNS + 10; z < -cornerEnd - 8; z += lampSpacing)
			addLampPost(sidewalkPosR, z, -1, 0);

		for (float z = -roadLongNS + 10; z < -cornerEnd - 8; z += lampSpacing)
			addLampPost(sidewalkPosL, z, 1, 0);

		for (float x = cornerEnd + 8; x < roadLongEW - 10; x += lampSpacing)
			addLampPost(x, roadW + sW, 0, -1);

		for (float x = cornerEnd + 8; x < roadLongEW - 10; x += lampSpacing)
			addLampPost(x, -(roadW + sW), 0, 1);

		for (float x = -roadLongEW + 10; x < -cornerEnd - 8; x += lampSpacing)
			addLampPost(x, roadW + sW, 0, -1);

		for (float x = -roadLongEW + 10; x < -cornerEnd - 8; x += lampSpacing)
			addLampPost(x, -(roadW + sW), 0, 1);

		streetLamps = BasicShape(lampV);



	}

	void Showroom::renderAll(const glm::mat4& viewProj)
	{

		lighting.applyToShader(BasicShape::getShaderProgram());


		outerGround.render(glm::mat4(1.0f), viewProj);
		ceiling.render(glm::mat4(1.0f), viewProj);
		exteriorWalls.render(glm::mat4(1.0f), viewProj);
		windowFrames.render(glm::mat4(1.0f), viewProj);
		mainDoorFrame.render(glm::mat4(1.0f), viewProj);
		columns.render(glm::mat4(1.0f), viewProj);
		streetAsphalt.render(glm::mat4(1.0f), viewProj);
		sidewalk.render(glm::mat4(1.0f), viewProj);
		streetLines.render(glm::mat4(1.0f), viewProj);
		roundabout.render(glm::mat4(1.0f), viewProj);
		streetLamps.render(glm::mat4(1.0f), viewProj);

		electricCarInRoom.render(viewProj);
		sportsCarInRoom.render(viewProj);
		suvCarInRoom.render(viewProj);


		for (auto& r : rooms)
			r->draw(viewProj);
		windowGlass.render(glm::mat4(1.0f), viewProj, 0.3f);
	}
	void Showroom::toggleLights()
	{
		lighting.toggleAllLights();

		if (lighting.lightsOn)
			audioPlay("assets/sounds/kill_switch.wav");
		else
			audioPlay("assets/sounds/kill_switch.wav");

		std::cout << " Lights toggled!" << std::endl;
	}

	glm::vec3 Showroom::checkCollision(glm::vec3 currentPos, glm::vec3 nextPos)
	{
		float margin = 0.5f;
		float podiumMargin = 0.4f;
		float columnMargin = 0.3f;

		float extMinX = exteriorBounds.minX;
		float extMaxX = exteriorBounds.maxX;
		float extMinZ = exteriorBounds.minZ;
		float extMaxZ = exteriorBounds.maxZ;
		float doorLeft = exteriorBounds.doorMinX;
		float doorRight = exteriorBounds.doorMaxX;
		float doorZ = exteriorBounds.doorZ;

		if (nextPos.z > extMinZ && nextPos.z < extMaxZ)
		{
			if (nextPos.x < extMinX + margin)
				nextPos.x = extMinX + margin;
			if (nextPos.x > extMaxX - margin)
				nextPos.x = extMaxX - margin;
		}

		if (nextPos.x > extMinX && nextPos.x < extMaxX)
		{
			if (nextPos.z < extMinZ + margin)
				nextPos.z = extMinZ + margin;
		}

		if (nextPos.x > extMinX && nextPos.x < extMaxX)
		{
			bool outsideDoor = (nextPos.x < doorLeft || nextPos.x > doorRight);
			if (outsideDoor)
			{
				if (currentPos.z < doorZ && nextPos.z > doorZ - margin)
					nextPos.z = doorZ - margin;
				else if (currentPos.z > doorZ && nextPos.z < doorZ + margin)
					nextPos.z = doorZ + margin;
			}
		}

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

				if (minDist == distLeft) nextPos.x = colXMin - columnMargin;
				else if (minDist == distRight) nextPos.x = colXMax + columnMargin;
				else if (minDist == distFront) nextPos.z = colZMax + columnMargin;
				else if (minDist == distBack) nextPos.z = colZMin - columnMargin;
			}
		}

		for (auto& room : rooms)
		{
			float xMin = room->centerOffset.x - room->size;
			float xMax = room->centerOffset.x + room->size;
			float zMin = room->centerOffset.z - room->size;
			float zMax = room->centerOffset.z + room->size;

			float doorHalfWidth = 2.5f;
			float dL = room->centerOffset.x - doorHalfWidth;
			float dR = room->centerOffset.x + doorHalfWidth;

			if (nextPos.z > zMin && nextPos.z < zMax)
			{
				if (currentPos.x <= xMin && nextPos.x > xMin - margin)
					nextPos.x = xMin - margin;
				else if (currentPos.x >= xMin && nextPos.x < xMin + margin)
					nextPos.x = xMin + margin;
			}

			if (nextPos.z > zMin && nextPos.z < zMax)
			{
				if (currentPos.x >= xMax && nextPos.x < xMax + margin)
					nextPos.x = xMax + margin;
				else if (currentPos.x <= xMax && nextPos.x > xMax - margin)
					nextPos.x = xMax - margin;
			}

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

			if (nextPos.x > xMin && nextPos.x < xMax)
			{
				bool outsideDoorWidth = (nextPos.x < dL || nextPos.x > dR) || !room->isDoorOpen();
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

					if (minDist == distLeft) nextPos.x = pXMin - podiumMargin;
					else if (minDist == distRight) nextPos.x = pXMax + podiumMargin;
					else if (minDist == distFront) nextPos.z = pZMax + podiumMargin;
					else if (minDist == distBack) nextPos.z = pZMin - podiumMargin;
				}
			}
		}

		return nextPos;
	}

	glm::vec3 Showroom::checkCollisionRadius(glm::vec3 currentPos, glm::vec3 nextPos, float radius)
	{
		float margin = radius;
		float podiumMargin = radius;
		float columnMargin = radius;

		float extMinX = exteriorBounds.minX;
		float extMaxX = exteriorBounds.maxX;
		float extMinZ = exteriorBounds.minZ;
		float extMaxZ = exteriorBounds.maxZ;
		float doorLeft = exteriorBounds.doorMinX;
		float doorRight = exteriorBounds.doorMaxX;
		float doorZ = exteriorBounds.doorZ;

		nextPos.x = std::max(extMinX + margin, std::min(nextPos.x, extMaxX - margin));
		nextPos.z = std::max(extMinZ + margin, nextPos.z);

		bool outsideMainDoor = (nextPos.x < doorLeft + margin || nextPos.x > doorRight - margin);
		if (outsideMainDoor)
		{
			nextPos.z = std::min(extMaxZ - margin, nextPos.z);
		}

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

				if (minDist == distLeft)       nextPos.x = colXMin - columnMargin;
				else if (minDist == distRight) nextPos.x = colXMax + columnMargin;
				else if (minDist == distFront) nextPos.z = colZMax + columnMargin;
				else                           nextPos.z = colZMin - columnMargin;
			}
		}

		for (auto& room : rooms)
		{
			float xMin = room->centerOffset.x - room->size;
			float xMax = room->centerOffset.x + room->size;
			float zMin = room->centerOffset.z - room->size;
			float zMax = room->centerOffset.z + room->size;

			bool nearRoom =
				nextPos.x > xMin - margin && nextPos.x < xMax + margin &&
				nextPos.z > zMin - margin && nextPos.z < zMax + margin;

			if (nearRoom)
			{
				nextPos.x = std::max(xMin + margin, std::min(nextPos.x, xMax - margin));

				float doorHalfWidth = 2.5f;
				float dL = room->centerOffset.x - doorHalfWidth;
				float dR = room->centerOffset.x + doorHalfWidth;

				if (room->doorAtMaxZ)
				{
					nextPos.z = std::max(zMin + margin, nextPos.z);

					bool doorAllowed = room->isDoorOpen() && (nextPos.x >= dL + margin && nextPos.x <= dR - margin);
					if (!doorAllowed)
						nextPos.z = std::min(zMax - margin, nextPos.z);
				}
				else
				{
					nextPos.z = std::min(zMax - margin, nextPos.z);

					bool doorAllowed = room->isDoorOpen() && (nextPos.x >= dL + margin && nextPos.x <= dR - margin);
					if (!doorAllowed)
						nextPos.z = std::max(zMin + margin, nextPos.z);
				}

				for (const auto& podium : room->podiumBounds)
				{
					float pXMin = podium.xCenter - podium.halfWidth;
					float pXMax = podium.xCenter + podium.halfWidth;
					float pZMin = podium.zCenter - podium.halfDepth;
					float pZMax = podium.zCenter + podium.halfDepth;

					bool insidePX = nextPos.x > pXMin - podiumMargin && nextPos.x < pXMax + podiumMargin;
					bool insidePZ = nextPos.z > pZMin - podiumMargin && nextPos.z < pZMax + podiumMargin;

					if (insidePX && insidePZ)
					{
						float distLeft = std::abs(nextPos.x - (pXMin - podiumMargin));
						float distRight = std::abs(nextPos.x - (pXMax + podiumMargin));
						float distFront = std::abs(nextPos.z - (pZMax + podiumMargin));
						float distBack = std::abs(nextPos.z - (pZMin - podiumMargin));

						float minDist = std::min({ distLeft, distRight, distFront, distBack });

						if (minDist == distLeft)       nextPos.x = pXMin - podiumMargin;
						else if (minDist == distRight) nextPos.x = pXMax + podiumMargin;
						else if (minDist == distFront) nextPos.z = pZMax + podiumMargin;
						else                           nextPos.z = pZMin - podiumMargin;
					}
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

	void Showroom::update(float dt)
	{
		for (auto& r : rooms)
			r->update(dt);

		const float carRadius = 1.2f;

		for (auto& r : rooms)
		{
			for (auto& carPtr : r->cars)
			{
				Car* car = carPtr.get();
				if (!car) continue;

				glm::vec3 oldPos = car->getPosition();
				car->update(dt);
				glm::vec3 newPos = car->getPosition();


				glm::vec3 fixed = newPos;

				float m = carRadius;
				fixed.x = std::max(exteriorBounds.minX + m, std::min(fixed.x, exteriorBounds.maxX - m));
				fixed.z = std::max(exteriorBounds.minZ + m, std::min(fixed.z, exteriorBounds.maxZ - m));

				float columnMargin = carRadius;
				for (const auto& col : columnBounds)
				{
					float colXMin = col.xCenter - col.halfSize;
					float colXMax = col.xCenter + col.halfSize;
					float colZMin = col.zCenter - col.halfSize;
					float colZMax = col.zCenter + col.halfSize;

					bool insideX = fixed.x > colXMin - columnMargin && fixed.x < colXMax + columnMargin;
					bool insideZ = fixed.z > colZMin - columnMargin && fixed.z < colZMax + columnMargin;

					if (insideX && insideZ)
					{
						float distLeft = std::abs(fixed.x - (colXMin - columnMargin));
						float distRight = std::abs(fixed.x - (colXMax + columnMargin));
						float distFront = std::abs(fixed.z - (colZMax + columnMargin));
						float distBack = std::abs(fixed.z - (colZMin - columnMargin));

						float minDist = std::min({ distLeft, distRight, distFront, distBack });

						if (minDist == distLeft) fixed.x = colXMin - columnMargin;
						else if (minDist == distRight) fixed.x = colXMax + columnMargin;
						else if (minDist == distFront) fixed.z = colZMax + columnMargin;
						else fixed.z = colZMin - columnMargin;
					}
				}

				if (fixed.x != newPos.x || fixed.z != newPos.z)
				{
					car->setPosition({ fixed.x, newPos.y, fixed.z });
					car->stopMovement();
				}
			}
		}
	}

	bool Showroom::toggleNearestRoomDoor(const glm::vec3& playerPos)
	{
		Room* best = nullptr;
		float bestDist = 1e9f;

		for (auto& r : rooms)
		{
			float doorZLocal = r->doorAtMaxZ ? r->size : -r->size;
			glm::vec3 doorCenter = r->centerOffset + glm::vec3(0.0f, 0.0f, doorZLocal);

			float dx = playerPos.x - doorCenter.x;
			float dz = playerPos.z - doorCenter.z;
			float dist = std::sqrt(dx * dx + dz * dz);

			if (dist < bestDist)
			{
				bestDist = dist;
				best = r.get();
			}
		}

		if (best && bestDist < 8.0f)
		{
			best->toggleDoor();

			if (best->isDoorOpen())
				audioPlay("assets/sounds/door_open.wav");
			else
				audioPlay("assets/sounds/door_close.wav");
			return true;
		}
		return false;
	}
}