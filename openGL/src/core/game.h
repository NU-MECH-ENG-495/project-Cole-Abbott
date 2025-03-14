// Game.h

#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "entities/player.h"
#include "graphics/renderer.h"
#include "world/world.h"
#include <memory>

class Game {
public:
    Game();
    ~Game();
    void run();
    std::shared_ptr<Player> player;
    std::shared_ptr<World> world;
    std::shared_ptr<Window> window;

private:
    void update();
    void render();


    std::shared_ptr<Renderer> renderer;


};

#endif