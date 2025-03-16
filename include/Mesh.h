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
    inline static constexpr unsigned int Stride = 8 * sizeof(float);
};

class Mesh {
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures);
    void draw(const ShaderProgram& shader) const;
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;
};

#endif