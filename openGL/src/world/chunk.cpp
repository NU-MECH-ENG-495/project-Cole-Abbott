#include "chunk.h"

glm::vec3 cubeVertices[6] = {
    // positions
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, 0.5f, -0.5f),
    glm::vec3(0.5f, 0.5f, -0.5f),
    glm::vec3(-0.5f, 0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
};

glm::vec2 texCoords[4] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
};

Chunk::Chunk(int x, int y, int z) : x(x), y(y), z(z)
{
    // Initialize blocks with some data (e.g., all blocks are solid)
    for (int i = 0; i < CHUNK_SIZE; ++i)
    {
        for (int j = 0; j < CHUNK_SIZE; ++j)
        {
            for (int k = 0; k < CHUNK_SIZE; ++k)
            {
                blocks[i][j][k] = 1; // Example: all blocks are solid
            }
        }
    }

    generateMesh();

    // // output the vertices
    // std::cout << "Chunk: " << x << ", " << y << ", " << z << std::endl;
    // for (int i = 0; i < vertices.size(); i += 6)
    // {
    //     std::cout << "Vert: " << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << std::endl;
    //     // std::cout << " Norm: " <<vertices[i + 3] << ", " << vertices[i + 4] << ", " << vertices[i + 5] << std::endl;
    // }
    // std::cout << "size of vertices: " << vertices.size() << std::endl;
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// generates a mesh with all the faces of the blocks in the chunk
// todo optimize this function to only generate faces for blocks that are exposed to air
void Chunk::generateMesh()
{
    vertices.clear();

    int indexCount = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (!isBlockSolid(x, y, z)) continue;
    
                if (!isBlockSolid(x, y, z - 1)) addFace(vertices, indices, {x, y, z}, {0, 0, -1}, texCoords, indexCount); // Front
                if (!isBlockSolid(x, y, z + 1)) addFace(vertices, indices, {x, y, z + 1}, {0, 0, 1}, texCoords, indexCount); // Back
                if (!isBlockSolid(x - 1, y, z)) addFace(vertices, indices, {x, y, z}, {-1, 0, 0}, texCoords, indexCount); // Left
                if (!isBlockSolid(x + 1, y, z)) addFace(vertices, indices, {x + 1, y, z}, {1, 0, 0}, texCoords, indexCount); // Right
                if (!isBlockSolid(x, y - 1, z)) addFace(vertices, indices, {x, y, z}, {0, -1, 0}, texCoords, indexCount); // Bottom
                if (!isBlockSolid(x, y + 1, z)) addFace(vertices, indices, {x, y + 1, z}, {0, 1, 0}, texCoords, indexCount); // Top

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

void Chunk::render(Shader* shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x * CHUNK_SIZE, y * CHUNK_HEIGHT, z * CHUNK_SIZE));
    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Chunk::addFace(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, 
    glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords[4], int& indexCount) {
// Compute right and up vectors based on normal direction
glm::vec3 right, up;

if (normal == glm::vec3(0, 0, 1)) {        // Front face
right = glm::vec3(1, 0, 0);
up = glm::vec3(0, 1, 0);
} else if (normal == glm::vec3(0, 0, -1)) { // Back face
right = glm::vec3(1, 0, 0);
up = glm::vec3(0, 1, 0);
} else if (normal == glm::vec3(1, 0, 0)) {  // Right face
right = glm::vec3(0, 0, 1);
up = glm::vec3(0, 1, 0);
} else if (normal == glm::vec3(-1, 0, 0)) { // Left face
right = glm::vec3(0, 0, 1);
up = glm::vec3(0, 1, 0);
} else if (normal == glm::vec3(0, 1, 0)) {  // Top face {x, y + 1, z}
right = glm::vec3(1, 0, 0);
up = glm::vec3(0, 0, 1);
} else if (normal == glm::vec3(0, -1, 0)) { // Bottom face
right = glm::vec3(1, 0, 0);
up = glm::vec3(0, 0, 1);
}

// Define 4 vertices using calculated directions
Vertex v0 = {position, normal, texCoords[0]};
Vertex v1 = {position + right, normal, texCoords[1]};
Vertex v2 = {position + right + up, normal, texCoords[2]};
Vertex v3 = {position + up, normal, texCoords[3]};

vertices.push_back(v0);
vertices.push_back(v1);
vertices.push_back(v2);
vertices.push_back(v3);

// Define indices
indices.push_back(indexCount);
indices.push_back(indexCount + 1);
indices.push_back(indexCount + 2);
indices.push_back(indexCount);
indices.push_back(indexCount + 2);
indices.push_back(indexCount + 3);

indexCount += 4;  // Move to next vertex index
}



bool Chunk::isBlockSolid(int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
    {
        return false;
    }

    return blocks[x][y][z] == 1;
}