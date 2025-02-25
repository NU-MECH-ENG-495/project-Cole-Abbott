// Game.h

#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "entities/player.h"
#include "graphics/renderer.h"
#include "world/world.h"

class Game {
public:
    Game();
    ~Game();
    void run();
    Player* player;

private:
    void update();
    void render(World* world);

    Window* window;
    Renderer* renderer;
    World* world;
};

#endif