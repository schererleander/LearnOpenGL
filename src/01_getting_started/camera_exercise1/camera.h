#ifndef CAMERA_H
#define CAMERA_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float fov;
    float sensitivity;
    float speed;

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH) : position(posX, posY, posZ), front(0.0f, 0.0f, -1.0f), up(upX, upY, upZ), right(0.0f), worldUp(upX, upY, upZ), yaw(yaw), pitch(pitch), fov(FOV), sensitivity(SENSITIVITY), speed(SPEED) {
        updateCameraVectors();
    }

    Camera(glm::vec3 position, glm::vec3 up, float yaw = YAW, float pitch = PITCH) : position(position), front(0.0f, 0.0f, -1.0f), up(up), right(0.0f), worldUp(up), yaw(yaw), pitch(pitch), fov(FOV), sensitivity(SENSITIVITY), speed(SPEED) {
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const {
            return glm::lookAt(position, position + front, up);
    }

    void ProcessKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = speed * deltaTime;

        if (direction == FORWARD) {
            position += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
        }

        if (direction == BACKWARD) {
            position -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * velocity;
        }

        if (direction == LEFT) {
            position -= right * velocity;
        }

        if (direction == RIGHT) {
            position += right * velocity;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }

            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        fov -= yoffset;

        if (fov < 1.0f) {
            fov = 1.0f;
        }

        if (fov > 45.0f) {
            fov = 45.0f;
        }
    }

private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        newFront.y = glm::sin(glm::radians(pitch));
        newFront.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif
