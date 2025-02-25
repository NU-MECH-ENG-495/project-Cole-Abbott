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
