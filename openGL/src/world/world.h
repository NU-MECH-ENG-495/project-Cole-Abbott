#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "chunk.h"
#include "graphics/shader.h"

class World {
public:
    World();
    ~World();

    void render(Shader* shader);

private:
    std::vector<Chunk*> chunks;
};

#endif // WORLD_H
