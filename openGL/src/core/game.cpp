#include "game.h"


/** 
 @brief Constructor for Game class. Initializes window and player.
*/
Game::Game() {
    window = std::make_shared<Window>(800, 600, "LearnOpenGL");
    window->setUserPointer(this);
    player = std::make_shared<Player>(glm::vec3(0.0f, 100.0f, 3.0f), -90, 0);
    renderer = std::make_shared<Renderer>();
    world = std::make_shared<World>();
    
}

/** 
 @brief Destructor for Game class. Cleans up resources.
*/
Game::~Game() {
    // all pointers are shared pointers, so no need to delete them
}

/** 
 @brief Main game loop. Processes input, updates game state, and renders the scene.
*/
void Game::run() {
    while (!window->shouldClose()) {
        window->processInput(*player);
        update();
        render();
        window->swapBuffers();
    }
}


/** 
 @brief Updates the game state by calling the player's update method.
*/
void Game::update() {
    player->update(world);
}

/** 
 @brief Renders the scene by clearing the renderer and drawing the player.
*/
void Game::render() {
    renderer->clear();
    renderer->draw(player, window, world);
}