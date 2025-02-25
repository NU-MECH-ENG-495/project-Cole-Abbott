#include "player.h"

Player::Player(glm::vec3 _playerPos, float _yaw, float _pitch)
    : playerPos(_playerPos), yaw(_yaw), pitch(_pitch), ourCamera(_playerPos, _yaw, _pitch) {
}

glm::mat4 Player::getCameraView() const {
    return ourCamera.cameraView();
}

void Player::processMouseMovement(double xpos, double ypos) {
    ourCamera.mouseMovement(xpos, ypos);
}

void Player::processKeyboardMovement(Camera_Movement direction, float deltaTime) {
    ourCamera.keyboardMovement(direction, deltaTime);
}

void Player::processInput(GLFWwindow* window) {
    static float lastFrame = 0.0f;

    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        processKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        processKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        processKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        processKeyboardMovement(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        processKeyboardMovement(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        processKeyboardMovement(DOWN, deltaTime);
}


void Player::update() {
    
}