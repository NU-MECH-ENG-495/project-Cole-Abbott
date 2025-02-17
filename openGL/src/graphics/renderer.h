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


class Renderer {
public:
    Renderer();
    ~Renderer();
    void clear();
    void draw(Player *player, Window *window);

private:
    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int VBO, VAO;
    Shader* lightingShader;
    Shader* lightCubeShader;
};

#endif