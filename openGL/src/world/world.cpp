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
            // chunks.push_back(new Chunk(x, 0, y));
            chunks.push_back(std::make_shared<Chunk>(x, 0, y));
        }
    }

}

World::~World() {
    // for (<Chunk>* chunk : chunks) {
    //     delete chunk;
    // }
}

/**
 * @brief renders all the chunks in the world
 * 
 * @param shader shader to use for rendering
 */
void World::render(Shader* shader, std::shared_ptr<Player> player) {
    // render the chunks that are in the player render distance
    //calculate the chunk coordinates of the player
    int chunkX = player->position.x / Chunk::CHUNK_SIZE;
    int chunkZ = player->position.z / Chunk::CHUNK_SIZE;
    //subtract 1 if negative to align with the chunk grid
    if (player->position.x < 0) {
        chunkX--;
        if (mod(player->position.x,Chunk::CHUNK_SIZE) == 0) {
            chunkX++;
        }
    }
    if (player->position.z < 0) {
        chunkZ--;
        if (mod(player->position.z,Chunk::CHUNK_SIZE) == 0) { 
            chunkZ++;
        }
    }

    // render / generate chunks in a square around the player
    for (int x = chunkX - player->renderDistance; x <= chunkX + player->renderDistance; ++x) {
        for (int z = chunkZ - player->renderDistance; z <= chunkZ + player->renderDistance; ++z) {
            // check if the chunk already exists
            bool found = false;
            for (std::shared_ptr<Chunk> chunk : chunks) {
                if (chunk->getX() == x && chunk->getZ() == z) {
                    found = true;
                    //render the chunk
                    chunk->render(shader);
                    break;
                }
            }
            // if not, create it
            if (!found) {
                // chunks.push_back(std::make_shared<Chunk>(x, 0, z));
            }
            
        }
    }

    // // delete chunks that are out of render distance
    // for (std::shared_ptr<Chunk> chunk : chunks) {
    //     if (abs(chunk->getX() - chunkX) > player->renderDistance || abs(chunk->getZ() - chunkZ) > player->renderDistance) {
    //         chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
    //     }
    // }
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

    
    
    for (std::shared_ptr<Chunk> chunk : chunks) {
        if (chunk->getX() == chunkX && chunk->getZ() == chunkZ) {
            return chunk->getBlock(mod(x,Chunk::CHUNK_SIZE), y, mod(z,Chunk::CHUNK_SIZE));
        }
    }
    return AIR;
}


/**
 * @brief sets the block type at the given position
 * 
 * @param x global x coordinate
 * @param y global y coordinate
 * @param z global z coordinate
 * @param type block type to set
 */
void World::setBlock(int x, int y, int z, BlockType type) {
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

    for (std::shared_ptr<Chunk> chunk : chunks) {
        if (chunk->getX() == chunkX && chunk->getZ() == chunkZ) {
            chunk->setBlock(mod(x,Chunk::CHUNK_SIZE), y, mod(z,Chunk::CHUNK_SIZE), type);
            break;
        }
    }
}

