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

    Player(glm::vec3 _playerPos, float _yaw, float _pitch);

    glm::mat4 getCameraView() const;
    void processMouseMovement(double xpos, double ypos);
    void processKeyboardMovement(Camera_Movement direction, float deltaTime);
    void processInput(GLFWwindow* window);
    void update();

private:
    Camera ourCamera;
};

#endif