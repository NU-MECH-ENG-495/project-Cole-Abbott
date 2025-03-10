#include "player.h"
#include "world/terrainGeneration.h"
#include "world/world.h"


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

/**
 * @brief Processes input from the keyboard and updates the player's velocity
 * 
 * @param window 
 */
void Player::processInput(GLFWwindow *window)
{
    float moveSpeed = 7.0f; // Movement speed
    glm::vec3 movement(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += getForwardVector();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= getForwardVector();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= getRightVector();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += getRightVector();
    if (glfwGetKey(window, GLFW_KEY_SPACE)) // jump / fly up
    {
        if (canFly)
            movement.y = 1.0f; // Fly up
        else if (canJump)
            velocity.y = 7.0f; // Jump velocity
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // crouch / fly down
    {
        if (canFly)
            movement.y = -1.0f; // Fly down
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // sprint
    {
        moveSpeed = canFly ? 40.0f : 12.0f; //move faster if flying
    }
    

    if (movement != glm::vec3(0.0f))
        movement = glm::normalize(movement);

    // Set velocity 
    velocity.x = movement.x * moveSpeed;
    velocity.z = movement.z * moveSpeed;
    if (canFly)
    velocity.y = movement.y * moveSpeed;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        //relsease the mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
        
}

/**
 * @brief Updates the player's position based on the velocity and collision detection
 * 
 * @param world 
 */
void Player::update(std::shared_ptr<World> world)
{

    float deltaTime = getDeltaTime();

    // Apply gravity
    if(!canFly){
        velocity.y -= 17.0f * deltaTime;
    }

    // Predict new position
    glm::vec3 newPos = position + velocity * deltaTime;

    // Check collision for each axis separately
    if (!isColliding(glm::vec3(newPos.x, position.y, position.z), size, world)){
        position.x = newPos.x;
    }
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
    if (!isColliding(glm::vec3(position.x, position.y, newPos.z), size, world)) {
        position.z = newPos.z;
    }
    ourCamera.setCameraPos(position);
    std::cout << "Player position: " << position.x << ", " << position.y << ", " << position.z;
    int yTerrain = PerlinNoise2(position.x, position.z) * 10 + 10;
    std::cout << " Terrain height: " << yTerrain << std::endl;

}

float Player::getDeltaTime()
{
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return deltaTime;
}

/**
 * @brief Checks if the player is colliding with any blocks in the world
 * 
 * @param newPos New position of the player
 * @param size Size of the player
 * @param world Pointer to the world object
 * @return true if the player is colliding with any blocks, false otherwise
 */
bool Player::isColliding(glm::vec3 newPos, glm::vec3 size, const std::shared_ptr<World> world)
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
    glm::vec3 front = ourCamera.cameraFront;
    front.y = 0;
    return glm::normalize(front);
}

glm::vec3 Player::getRightVector()
{
    return glm::normalize(glm::cross(getForwardVector(), ourCamera.cameraUp));
}