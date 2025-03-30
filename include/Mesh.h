#ifndef __MESH__
#define __MESH__

#include <glm/glm.hpp>
#include <vector>

#include "Shader.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
friend class Model;
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<unsigned int>&& textureIndices);
    ~Mesh();
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);

private:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<unsigned int> mTextureIndices;

    unsigned int mVBO;
    unsigned int mEBO;
    unsigned int mVAO;
};

#endif