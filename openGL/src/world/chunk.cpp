#include "chunk.h"
#include "terrainGeneration.h"

glm::vec3 cubeVertices[6] = {
    // positions
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, 0.5f, -0.5f),
    glm::vec3(0.5f, 0.5f, -0.5f),
    glm::vec3(-0.5f, 0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
};

// glm::vec2 topTexCoords[4] = {
//     glm::vec2(0.0f, 0.0f),
//     glm::vec2(1.0f, 0.0f),
//     glm::vec2(1.0f, 1.0f),
//     glm::vec2(0.0f, 1.0f),
// };
// glm::vec2 topTexCoords[4] = {
//     glm::vec2(0.0f, 0.0f),
//     glm::vec2(0.25f, 0.0f),
//     glm::vec2(0.25f, 0.25f),
//     glm::vec2(0.0f, 0.25f),
// };
// glm::vec2 sideTexCoords[4] = {
//     glm::vec2(0.25f, 0.25f),
//     glm::vec2(0.5f, 0.25f),
//     glm::vec2(0.5f, 0.0f),
//     glm::vec2(0.25f, 0.0f),
// };

glm::vec2 atlasIndex[4] = {
    glm::vec2(0.0f, 0.25f),
    glm::vec2(0.25f, 0.25f),
    glm::vec2(0.25f, 0.0f),
    glm::vec2(0.0f, 0.0f),
};

/**
 * @brief Construct a new Chunk:: Chunk object
 *
 * @param x x coordinate of the chunk
 * @param y y coordinate of the chunk
 * @param z z coordinate of the chunk
 */
Chunk::Chunk(int x, int y, int z) : x(x), y(y), z(z)
{
    // Initialize blocks with some data (e.g., all blocks are solid)
    for (int i = 0; i < CHUNK_SIZE; ++i) // x
    {
        for (int k = 0; k < CHUNK_SIZE; ++k) // z
        {
            // calculate height with perlin noise
            double height = PerlinNoise2((x * CHUNK_SIZE + i), (z * CHUNK_SIZE + k)) * 40 + 64; // scaled to 0-20

            if (height > WATER_LEVEL + 1)
            {                                          // generate stone, dirt, grass, air
                for (int j = 0; j < CHUNK_HEIGHT; ++j) // z
                {
                    if (j <= height - 3)
                    {
                        blocks[i][j][k] = STONE;
                    }
                    else if (j <= height - 1)
                    {
                        blocks[i][j][k] = DIRT;
                    }
                    else if (j <= height)
                    {
                        blocks[i][j][k] = GRASS;
                    }
                    else
                    {
                        blocks[i][j][k] = AIR;
                    }
                }
            }
            else // Generate stone, sand, water, air
            {
                for (int j = 0; j < CHUNK_HEIGHT; ++j) // z
                {
                    if (j <= height - 3)
                    {
                        blocks[i][j][k] = STONE;
                    }
                    else if (j <= height + 1)
                    {
                        blocks[i][j][k] = SAND;
                    }
                    else if (j <= WATER_LEVEL)
                    {
                        blocks[i][j][k] = WATER;
                    }
                    else
                    {
                        blocks[i][j][k] = AIR;
                    }
                }
            }
        }
    }

    generateMesh();
}

/**
 * @brief Destroy the Chunk:: Chunk object
 *
 */
Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

/**
 * @brief Generates the mesh for the chunk
 *
 */
void Chunk::generateMesh()
{
    vertices.clear();

    int indexCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                BlockType type = blocks[x][y][z];
                if (!isBlockSolid(x, y, z))
                    continue;

                if (!isBlockSolid(x, y, z - 1))
                    addFace(vertices, indices, {x, y, z}, {0, 0, -1}, type, indexCount); // Front
                if (!isBlockSolid(x, y, z + 1))
                    addFace(vertices, indices, {x, y, z + 1}, {0, 0, 1}, type, indexCount); // Back
                if (!isBlockSolid(x - 1, y, z))
                    addFace(vertices, indices, {x, y, z}, {-1, 0, 0}, type, indexCount); // Left
                if (!isBlockSolid(x + 1, y, z))
                    addFace(vertices, indices, {x + 1, y, z}, {1, 0, 0}, type, indexCount); // Right
                if (!isBlockSolid(x, y - 1, z))
                    addFace(vertices, indices, {x, y, z}, {0, -1, 0}, type, indexCount); // Bottom
                if (!isBlockSolid(x, y + 1, z))
                    addFace(vertices, indices, {x, y + 1, z}, {0, 1, 0}, type, indexCount); // Top
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief Renders the chunk
 *
 * @param shader shader to use for rendering
 */
void Chunk::render(Shader *shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x * CHUNK_SIZE, y * CHUNK_HEIGHT, z * CHUNK_SIZE));
    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/**
 * @brief Adds a face to the chunk mesh
 *
 * @param vertices Vector of vertices, this will be pushed to the VBO
 * @param indices Vector of indices, this will be pushed to the EBO
 * @param position position of the block in the chunk
 * @param normal normal vector to the face
 * @param texCoords textrure coordinates of the face
 * @param indexCount used to keep track of the index of the next vertex
 */
void Chunk::addFace(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,
                    glm::vec3 position, glm::vec3 normal, BlockType type, int &indexCount)
{
    // Compute right and up vectors based on normal direction
    glm::vec3 right, up;
    BlockSides side;

    // Determine right and up vectors based on normal direction
    if (normal == glm::vec3(0, 0, 1))
    { // Front face
        right = glm::vec3(1, 0, 0);
        up = glm::vec3(0, 1, 0);
        side = SIDE;
    }
    else if (normal == glm::vec3(0, 0, -1))
    { // Back face
        right = glm::vec3(1, 0, 0);
        up = glm::vec3(0, 1, 0);
        side = SIDE;
    }
    else if (normal == glm::vec3(1, 0, 0))
    { // Right face
        right = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);
        side = SIDE;
    }
    else if (normal == glm::vec3(-1, 0, 0))
    { // Left face
        right = glm::vec3(0, 0, 1);
        up = glm::vec3(0, 1, 0);
        side = SIDE;
    }
    else if (normal == glm::vec3(0, 1, 0))
    { // Top face
        right = glm::vec3(1, 0, 0);
        up = glm::vec3(0, 0, 1);
        side = TOP;
    }
    else if (normal == glm::vec3(0, -1, 0))
    { // Bottom face
        right = glm::vec3(1, 0, 0);
        up = glm::vec3(0, 0, 1);
        side = BOTTOM;
    }

    glm::vec2 texCoords[4];
    calcTexCoords(type, texCoords, side);

    // Define 4 vertices using calculated directions
    Vertex v0 = {position, normal, texCoords[0]};
    Vertex v1 = {position + right, normal, texCoords[1]};
    Vertex v2 = {position + right + up, normal, texCoords[2]};
    Vertex v3 = {position + up, normal, texCoords[3]};

    // push vertices to the vector
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    // push indices to the vector
    indices.push_back(indexCount);
    indices.push_back(indexCount + 1);
    indices.push_back(indexCount + 2);
    indices.push_back(indexCount);
    indices.push_back(indexCount + 2);
    indices.push_back(indexCount + 3);

    indexCount += 4; // Move to next vertex index
}

/**
 * @brief checks if a block is solid
 *
 * @param x x coordinate of the block in the chunk
 * @param y y coordinate of the block in the chunk
 * @param z z coordinate of the block in the chunk
 * @return true if the block is solid, false if the block is an air block
 */
bool Chunk::isBlockSolid(int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
    {
        return false;
    }

    return blocks[x][y][z] != AIR;
}

/**
 * @brief sets the block at the given coordinates to the given type
 *
 * @param x x coordinate of the block in the chunk
 * @param y y coordinate of the block in the chunk
 * @param z z coordinate of the block in the chunk
 * @param type type of the block
 */
void Chunk::setBlock(int x, int y, int z, BlockType type)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
    {
        return;
    }

    blocks[x][y][z] = type;
    generateMesh();
}

/**
 * @brief calculates the texture coordinates for a block
 *
 * @param type type of the block
 * @param texCoords array of texture coordinates
 */
void Chunk::calcTexCoords(BlockType type, glm::vec2 texCoords[4], BlockSides side)
{
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);
    float cubeSize = 0.25f;
    switch (type)
    {
    case DIRT: // 0,1, same on all sides
        offset.y += cubeSize;
        break;
    case GRASS: // 0,0 on top, 1,0 on sides, 0,1 on bottom
        if (side == SIDE)
            offset.x += cubeSize;
        else if (side == BOTTOM)
            offset.y += cubeSize;
        break;
    case STONE: // 1,1 on all sides
        offset.x += cubeSize;
        offset.y += cubeSize;
        break;
    case WATER: // 3,0 on all sides
        offset.x += 3 * cubeSize;
        break;
    case SAND: // 2,0 on all sides
        offset.x += 2 * cubeSize;
        break;
    case OAK_LOG: // 2,1 on sides, 3,1 on top,bottom
        offset.y += cubeSize;
        if (side == SIDE)
            offset.x += 2 * cubeSize;
        else if (side == TOP || side == BOTTOM)
            offset.x += 3 * cubeSize;
        break;
    default:
        break;
    }

    // set the texture coordinates
    texCoords[0] = atlasIndex[0] + offset;
    texCoords[1] = atlasIndex[1] + offset;
    texCoords[2] = atlasIndex[2] + offset;
    texCoords[3] = atlasIndex[3] + offset;
}