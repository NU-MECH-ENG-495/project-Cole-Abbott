#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "world/world.h"

class Player
{
public:
    glm::vec3 position;
    glm::vec3 velocity;  // Movement velocity
    glm::vec3 size = glm::vec3(0.6f, 1.8f, 0.6f); // Approximate player size


    float yaw, pitch;

    Player(glm::vec3 _playerPos, float _yaw, float _pitch);

    glm::mat4 getCameraView() const;
    void processMouseMovement(double xpos, double ypos);

    void processInput(GLFWwindow* window);
    void update(World *world);

private:
    float lastFrame = 0.0f;
    Camera ourCamera;
    bool canJump = true;
    float getDeltaTime();
    bool isColliding(glm::vec3 newPos, glm::vec3 size, const World *world);
    glm::vec3 getForwardVector();
    glm::vec3 getRightVector();
};

#endif