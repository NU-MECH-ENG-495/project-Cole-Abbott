#include "world.h"

World::World() {
    // Create some chunks (example: create a 2x2 grid of chunks)
    for (int x = -4; x < 4; ++x) {
        for (int y = -4; y < 4; ++y) {
            chunks.push_back(new Chunk(x, 0, y));
        }
    }

}

World::~World() {
    for (Chunk* chunk : chunks) {
        delete chunk;
    }
}

void World::render(Shader* shader) {
    for (Chunk* chunk : chunks) {
        chunk->render(shader);
    }
}

//
BlockType World::getBlock(int x, int y, int z) const {
    // Find the chunk that contains the block
    int chunkX = x / Chunk::CHUNK_SIZE;
    int chunkZ = z / Chunk::CHUNK_SIZE;
    
    for (Chunk* chunk : chunks) {
        if (chunk->getX() == chunkX && chunk->getZ() == chunkZ) {
            return chunk->getBlock(x % Chunk::CHUNK_SIZE, y, z % Chunk::CHUNK_SIZE);
        }
    }
    return AIR;
}