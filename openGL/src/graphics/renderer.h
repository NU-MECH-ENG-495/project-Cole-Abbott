#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include "shader.h"
// #include "utils/utils.h"
#include "vertices.h"
#include "entities/player.h"
#include "core/window.h"
#include "world/world.h"


class Renderer {
public:
    Renderer();
    ~Renderer();
    void clear();
    void draw(Player *player, Window *window, World *world);

private:
    unsigned int grassTexture;
    unsigned int grassSide;
    unsigned int VBO, VAO;
    Shader* blockShader;
};

#endif