#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "chunk.h"
#include "graphics/shader.h"
#include "blockTypes.h"
#include "entities/player.h"



class World {
public:
    World();
    ~World();

    void render(Shader* shader, std::shared_ptr<Player> player);
    BlockType getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockType type);

private:
    std::vector<std::shared_ptr<Chunk>> chunks;
};

#endif // WORLD_H
