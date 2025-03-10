#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "entities/camera.h"
#include "entities/player.h"
#include "graphics/shader.h"

// #include "graphics/renderer.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"


class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();
    void swapBuffers();
    bool shouldClose();
    void processInput(Player& player);
    void setUserPointer(void* ptr);
    void setWindowSize(int new_width, int new_height);

    
    
    int width, height;
    
    private: 
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


    GLFWwindow* window;
    std::string title;

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

};


#endif