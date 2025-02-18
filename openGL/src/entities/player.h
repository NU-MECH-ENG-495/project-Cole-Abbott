#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

class Player
{
public:
    glm::vec3 playerPos;
    float yaw, pitch;
    bool flashLightOn = false;

    Player(glm::vec3 _playerPos, float _yaw, float _pitch)
        : playerPos(_playerPos), yaw(_yaw), pitch(_pitch), ourCamera(_playerPos, _yaw, _pitch)
    {
    }

    glm::mat4 getCameraView() const
    {
        return ourCamera.cameraView();
    }

    glm::vec3 getCameraFront() const
    {
        return ourCamera.cameraFront;
    }

    void processMouseMovement(double xpos, double ypos)
    {
        ourCamera.mouseMovement(xpos, ypos);
    }

    void manageMovementInput(Camera_Movement direction, float deltaTime)
    {
        ourCamera.keyboardMovement(direction, deltaTime);
    }

    void update()
    {
        playerPos = ourCamera.cameraPos;
    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        processMouseMovement(xpos, ypos);
    }

    void processInput(GLFWwindow *window)
    {
        static float deltaTime = 0.0f; // Time between current frame and last frame
        static float lastFrame = 0.0f; // Time of last frame

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        const float cameraSpeed = 2.5 * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            manageMovementInput(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            manageMovementInput(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            manageMovementInput(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            manageMovementInput(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            manageMovementInput(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            manageMovementInput(DOWN, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            flashLightOn = true;
        else
            flashLightOn = false;

        
    }

private:
    Camera ourCamera;
};

#endif