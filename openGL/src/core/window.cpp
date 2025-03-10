#include "window.h"
#include "game.h"

Window::Window(int width, int height, const std::string &title)
    : width(width), height(height), title(title)
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST); // enable depth testing

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwSetCursorPosCallback(window, mouse_callback);            // set mouse movement callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);   // set mouse button callback
}


Window::~Window()
{
    glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow *window, int new_width, int new_height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, new_width, new_height);
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->window->setWindowSize(new_width, new_height);

}

void Window::setWindowSize(int new_width, int new_height)
{
    width = new_width;
    height = new_height;

}

/**
 * @brief Swaps the buffers of the window.
 * 
 */
void Window::swapBuffers()
{
    glfwSwapBuffers(window);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Window::processInput(Player& player)
{
    player.processInput(window);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
    // return false;
}


void Window::setUserPointer(void* ptr) {
    glfwSetWindowUserPointer(window, ptr);
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));  // Retrieve Game instance
    if (game) {
        game->player->processMouseMovement(xpos, ypos);
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));  // Retrieve Game instance
    if (game) {
        game->player->processMouseButton(button, action, game->world);
    }
}