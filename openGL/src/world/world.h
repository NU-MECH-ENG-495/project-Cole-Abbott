#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "chunk.h"
#include "graphics/shader.h"
#include "blockTypes.h"



class World {
public:
    World();
    ~World();

    void render(Shader* shader);
    BlockType getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockType type);

private:
    std::vector<Chunk*> chunks;
};

#endif // WORLD_H
