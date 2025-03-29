#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures) :
    mVertices{std::move(vertices)},
    mIndices{std::move(indices)},
    mTextures{std::move(textures)}
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
    mVertices{std::move(other.mVertices)},
    mIndices{std::move(other.mIndices)},
    mTextures{std::move(other.mTextures)},
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
    mTextures = std::move(temp.mTextures);
    std::swap(mVAO, temp.mVAO);
    std::swap(mVBO, temp.mVBO);
    std::swap(mEBO, temp.mEBO);
    return *this;
}

void Mesh::draw(const ShaderProgram& shader) const {
    if(mVAO == 0) {
        throw std::runtime_error("Trying to draw deleted Mesh");
    }
    std::size_t numDiffuse = 0;
    std::size_t numSpecular  = 0;
    for(std::size_t i = 0;i < mTextures.size();i++) {
        mTextures[i].bind(GL_TEXTURE0 + i);
        switch(mTextures[i].getType()) {
            case Texture::Type::Diffuse:
                shader.setInt("diffuseTexture" + std::to_string(numDiffuse++), i);
                break;
            case Texture::Type::Specular:
                shader.setInt("specularTexture" + std::to_string(numSpecular++), i);
                break;
        }
    }
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
