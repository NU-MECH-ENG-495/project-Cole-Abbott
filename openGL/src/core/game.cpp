#include "game.h"


/** 
 @brief Constructor for Game class. Initializes window and player.
*/
Game::Game() {
    window = new Window(800, 600, "LearnOpenGL");
    window->setUserPointer(this);
    player = new Player(glm::vec3(0.0f, 0.0f, 3.0f), -90, 0);
    renderer = new Renderer();
    world = new World();
}

/** 
 @brief Destructor for Game class. Cleans up resources.
*/
Game::~Game() {
    delete window;
    delete player;
    delete renderer;
}

/** 
 @brief Main game loop. Processes input, updates game state, and renders the scene.
*/
void Game::run() {
    while (!window->shouldClose()) {
        window->processInput(*player);
        update();
        render(world);
        window->swapBuffers();
    }
}


/** 
 @brief Updates the game state by calling the player's update method.
*/
void Game::update() {
    player->update();
}

/** 
 @brief Renders the scene by clearing the renderer and drawing the player.
*/
void Game::render(World *world) {
    renderer->clear();
    renderer->draw(player, window, world);
}