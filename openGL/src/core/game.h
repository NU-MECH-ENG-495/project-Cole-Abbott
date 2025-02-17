// Game.h

#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "entities/player.h"
#include "graphics/renderer.h"

class Game {
public:
    Game();
    ~Game();
    void run();
    Player* player;

private:
    void update();
    void render();

    Window* window;
    Renderer* renderer;
};

#endif