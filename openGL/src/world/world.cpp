#include "world.h"


float mod(float a, float b) {
    return a - b * floor(a / b);
}

/**
 * @brief Construct a new World:: World object
 * 
 */ 
World::World() {
    // Create some chunks (example: create a 2x2 grid of chunks)
    for (int x = -10; x < 10; ++x) {
        for (int y = -10; y < 10; ++y) {
            chunks.push_back(new Chunk(x, 0, y));
        }
    }

}

World::~World() {
    for (Chunk* chunk : chunks) {
        delete chunk;
    }
}

/**
 * @brief renders all the chunks in the world
 * 
 * @param shader shader to use for rendering
 */
void World::render(Shader* shader) {
    for (Chunk* chunk : chunks) {
        chunk->render(shader);
    }
}

/**
 * @brief gets the block type at the given position
 * 
 * @param x global x coordinate
 * @param y global y coordinate
 * @param z global z coordinate
 * @return BlockType 
 */
BlockType World::getBlock(int x, int y, int z) const {
    // Find the chunk that contains the block
    int chunkX = x / Chunk::CHUNK_SIZE;
    int chunkZ = z / Chunk::CHUNK_SIZE;


    //subtract 1 if negative to align with the chunk grid
    if (x < 0) {
        chunkX--;
        if (mod(x,Chunk::CHUNK_SIZE) == 0) {
            chunkX++;
        }
    }
    if (z < 0) {
        chunkZ--;
        if (mod(z,Chunk::CHUNK_SIZE) == 0) {
            chunkZ++;
        }
    }

    
    
    for (Chunk* chunk : chunks) {
        if (chunk->getX() == chunkX && chunk->getZ() == chunkZ) {
            return chunk->getBlock(mod(x,Chunk::CHUNK_SIZE), y, mod(z,Chunk::CHUNK_SIZE));
        }
    }
    return AIR;
}


