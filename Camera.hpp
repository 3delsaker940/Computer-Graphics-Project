#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Example {
    enum Camera_Mode { FREE_ROAM, ORBIT };

    class Camera {
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;
        float MovementSpeed;
        float MouseSensitivity;
        Camera_Mode Mode;

        glm::vec3 Target;
        float Distance;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.5f, 3.0f))
            : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Mode(FREE_ROAM), Target(glm::vec3(0.0f)), Distance(4.0f)
        {
            Position = position;
            WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            Yaw = -90.0f;
            Pitch = 0.0f;
            updateCameraVectors();
        }

        glm::mat4 GetViewMatrix() {
            if (Mode == ORBIT) {
                Position.x = Target.x + Distance * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
                Position.y = Target.y + Distance * sin(glm::radians(Pitch));
                Position.z = Target.z + Distance * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
                return glm::lookAt(Position, Target, WorldUp);
            }
            return glm::lookAt(Position, Position + Front, Up);
        }

        void ProcessKeyboard(std::string direction, float deltaTime) {
            if (Mode != FREE_ROAM) return;
            float velocity = MovementSpeed * deltaTime;
            if (direction == "FORWARD") Position += Front * velocity;
            if (direction == "BACKWARD") Position -= Front * velocity;
            if (direction == "LEFT") Position -= Right * velocity;
            if (direction == "RIGHT") Position += Right * velocity;
        }

        void ProcessMouseMovement(float xoffset, float yoffset) {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;
            Yaw += xoffset;
            Pitch += yoffset;
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
            updateCameraVectors();
        }

    private:
        void updateCameraVectors() {
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }
    };
}
#endif