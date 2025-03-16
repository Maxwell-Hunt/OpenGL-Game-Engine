#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures) :
    vertices{std::move(vertices)},
    indices{std::move(indices)},
    textures{std::move(textures)}
{
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::Stride, (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::Stride, (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::Stride, (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(const ShaderProgram& shader) const {
    std::size_t numDiffuse = 0;
    std::size_t numSpecular  = 0;
    for(std::size_t i = 0;i < textures.size();i++) {
        textures[i].bind(GL_TEXTURE0 + i);
        switch(textures[i].getType()) {
            case Texture::Type::Diffuse:
                shader.setInt("diffuseTexture" + std::to_string(numDiffuse++), i);
                break;
            case Texture::Type::Specular:
                shader.setInt("specularTexture" + std::to_string(numSpecular++), i);
                break;
        }
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
