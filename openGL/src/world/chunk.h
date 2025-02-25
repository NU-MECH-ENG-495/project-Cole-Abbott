#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "graphics/shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Chunk
{
public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 2;

    Chunk(int x, int y, int z);
    ~Chunk();

    void generateMesh();
    void render(Shader *shader);

private:
    int x, y, z;

    // stores the block data for the chunk, index by [x][y][z] in local coordinates
    int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GLuint VAO, VBO, EBO;
    Shader *shader;

    void addFace(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords[4], int &indexCount);

    bool isBlockSolid(int x, int y, int z);
};

#endif // CHUNK_H
