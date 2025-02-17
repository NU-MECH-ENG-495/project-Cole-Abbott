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

    Player(glm::vec3 _playerPos, float _yaw, float _pitch)
        : playerPos(_playerPos), yaw(_yaw), pitch(_pitch), ourCamera(_playerPos, _yaw, _pitch)
    {
    }

    glm::mat4 getCameraView() const
    {
        return ourCamera.cameraView();
    }

    void processMouseMovement(double xpos, double ypos)
    {
        ourCamera.mouseMovement(xpos, ypos);
    }

    void processKeyboardMovement(Camera_Movement direction, float deltaTime)
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

private:
    Camera ourCamera;
};

#endif