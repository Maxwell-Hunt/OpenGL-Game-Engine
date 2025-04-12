#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(LightingType lightingType, Color color, std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<unsigned int>&& textureIndices) :
    mLightingType{lightingType},
    mColor{color},
    mVertices{std::move(vertices)},
    mIndices{std::move(indices)},
    mTextureIndices{std::move(textureIndices)}
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if(mVAO != 0) glDeleteVertexArrays(1, &mVAO);
    if(mVBO != 0) glDeleteBuffers(1, &mVBO);
    if(mEBO != 0) glDeleteBuffers(1, &mEBO);
}

Mesh::Mesh(Mesh&& other) :
    mLightingType{other.mLightingType},
    mColor{other.mColor},
    mVertices{std::move(other.mVertices)},
    mIndices{std::move(other.mIndices)},
    mTextureIndices{std::move(other.mTextureIndices)},
    mVBO{other.mVBO},
    mEBO{other.mEBO},
    mVAO{other.mVAO}
{
    other.mVAO = 0;
    other.mVBO = 0;
    other.mEBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) {
    if(this == &other) return *this;
    Mesh temp(std::move(other));
    mVertices = std::move(temp.mVertices);
    mIndices = std::move(temp.mIndices);
    mTextureIndices = std::move(temp.mTextureIndices);
    std::swap(mLightingType, temp.mLightingType);
    std::swap(mColor, other.mColor);
    std::swap(mVAO, temp.mVAO);
    std::swap(mVBO, temp.mVBO);
    std::swap(mEBO, temp.mEBO);
    return *this;
}
