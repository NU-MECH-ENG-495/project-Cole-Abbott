#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw, pitch;
    bool firstMouse = true;
    float lastX = 400, lastY = 300;

    Camera(glm::vec3 _cameraPos, float _yaw, float _pitch)
    {
        cameraPos = _cameraPos;
        yaw = _yaw;
        pitch = _pitch;
    }

    glm::mat4 cameraView() const
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void mouseMovement(double xpos, double ypos)
    {
        if (firstMouse) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    void keyboardMovement(Camera_Movement direction, float deltaTime)
    {
        const float cameraSpeed = 2.5 * deltaTime; // adjust accordingly
        if (direction == FORWARD)
            // cameraPos += cameraSpeed * cameraFront; //FLY camera
            cameraPos += glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z)) * cameraSpeed;
        if (direction == BACKWARD)
            // cameraPos -= cameraSpeed * cameraFront; //FLY camera
            cameraPos -= glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z)) * cameraSpeed;

        if (direction == LEFT)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (direction == RIGHT)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (direction == UP)
            cameraPos += cameraSpeed * cameraUp;
        if (direction == DOWN)
            cameraPos -= cameraSpeed * cameraUp;
    }
};

#endif // CAMERA_H