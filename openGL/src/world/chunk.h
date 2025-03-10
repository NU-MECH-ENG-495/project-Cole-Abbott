#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "graphics/shader.h"
#include "blockTypes.h"



struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

enum BlockSides
{
    TOP,
    SIDE,
    BOTTOM
};

class Chunk
{
public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 128;
    static const int WATER_LEVEL = 56;

    Chunk(int x, int y, int z);
    ~Chunk();

    void generateMesh();
    void render(Shader *shader);
    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    BlockType getBlock(int x, int y, int z) const { return blocks[x][y][z]; }
    void setBlock(int x, int y, int z, BlockType type);

private:
    int x, y, z;

    // stores the block data for the chunk, index by [x][y][z] in local coordinates
    BlockType blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GLuint VAO, VBO, EBO;
    Shader *shader;

    void addFace(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                 glm::vec3 position, glm::vec3 normal, BlockType type, int &indexCount);

    bool isBlockSolid(int x, int y, int z);
    void calcTexCoords(BlockType type, glm::vec2 texCoords[4], BlockSides side);
};

#endif // CHUNK_H
