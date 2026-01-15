#include "Camera.hpp"

#include <glm/ext.hpp>
#include <cmath>

void Camera::handleInput(GLFWwindow *window, float dt)
{
    const float rotSpeed = 1.5f;
    const float zoomSpeed = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw += rotSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw -= rotSpeed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pitch += rotSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pitch -= rotSpeed * dt;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        distance += zoomSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        distance -= zoomSpeed * dt;

    if (distance < 0.5f)
        distance = 0.5f;
}

glm::mat4 Camera::getViewProjection() const
{
    glm::vec3 pos;
    pos.x = target.x + distance * std::cos(pitch) * std::cos(yaw);
    pos.y = target.y + distance * std::cos(pitch) * std::sin(yaw);
    pos.z = target.z + distance * std::sin(pitch);

    glm::mat4 view = glm::lookAt(
        pos,
        target,
        glm::vec3(0, 0, 1));

    glm::mat4 proj = glm::perspective(
        glm::radians(60.0f),
        aspect,
        0.1f,
        100.0f);

    return proj * view;
}