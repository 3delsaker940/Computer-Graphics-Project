#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
	float distance = 2.5f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float aspect = 1.0f;

	glm::vec3 target = { 0,0,0 };

public:
	void setAspect(float a) { aspect = a; }
	void setOrbitTarget(const glm::vec3& t) { target = t; }

	void handleInput(GLFWwindow* window, float dt);
	glm::mat4 getViewProjection() const;
};