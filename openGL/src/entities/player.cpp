#include "player.h"

Player::Player(glm::vec3 _playerPos, float _yaw, float _pitch)
    : position(_playerPos), yaw(_yaw), pitch(_pitch), ourCamera(_playerPos, _yaw, _pitch)
{
}

glm::mat4 Player::getCameraView() const
{
    return ourCamera.cameraView();
}

void Player::processMouseMovement(double xpos, double ypos)
{
    ourCamera.mouseMovement(xpos, ypos);
}


void Player::processInput(GLFWwindow *window)
{
    const float moveSpeed = 5.0f; // Movement speed
    glm::vec3 movement(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += getForwardVector();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= getForwardVector();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= getRightVector();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += getRightVector();
    if (glfwGetKey(window, GLFW_KEY_SPACE) && canJump)
    {
        velocity.y = 5.0f; // Jump velocity
    }

    // movement = glm::normalize(movement) * moveSpeed;

    // Set velocity (X & Z only, Y is handled by gravity)
    velocity.x = movement.x * moveSpeed;
    velocity.z = movement.z * moveSpeed;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Player::update(World *world)
{

    float deltaTime = getDeltaTime();

    // Apply gravity
    velocity.y -= 15.0f * deltaTime;

    // Predict new position
    glm::vec3 newPos = position + velocity * deltaTime;

    // Check collision for each axis separately
    if (!isColliding(glm::vec3(newPos.x, position.y, position.z), size, world))
        position.x = newPos.x;
    if (!isColliding(glm::vec3(position.x, newPos.y, position.z), size, world))
    {
        position.y = newPos.y; // Move player down
        canJump = false;       // Player is in the air
    }
    else
    {
        velocity.y = 0; // Stop falling when hitting the ground
        canJump = true; // Allow player to jump
    }
    if (!isColliding(glm::vec3(position.x, position.y, newPos.z), size, world))
        position.z = newPos.z;

    ourCamera.setCameraPos(position);
}

float Player::getDeltaTime()
{
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return deltaTime;
}

bool Player::isColliding(glm::vec3 newPos, glm::vec3 size, const World *world)
{
    int minX = floor(newPos.x - size.x / 2);
    int maxX = floor(newPos.x + size.x / 2);
    int minY = floor(newPos.y - size.y);
    int maxY = floor(newPos.y);
    int minZ = floor(newPos.z - size.z / 2);
    int maxZ = floor(newPos.z + size.z / 2);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            for (int z = minZ; z <= maxZ; z++)
            {
                if (world->getBlock(x, y, z) != AIR)
                {
                    return true; // Collision detected
                }
            }
        }
    }
    return false;
}

glm::vec3 Player::getForwardVector()
{
    return ourCamera.cameraFront;
}

glm::vec3 Player::getRightVector()
{
    return glm::normalize(glm::cross(ourCamera.cameraFront, ourCamera.cameraUp));
}