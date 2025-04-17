#include "Model.h"

#include <iostream>

Model::Model(std::filesystem::path&& directory, std::vector<Mesh>&& meshes, std::vector<Texture>&& textures) :
    mDirectory{std::move(directory)},
    mMeshes{std::move(meshes)},
    mTextures{std::move(textures)} {}

void Model::draw(const ShaderProgram& shader) const {
    for(const Mesh& mesh : mMeshes) {
        drawMesh(shader, mesh);
    }
}

void Model::drawMesh(const ShaderProgram& shader, const Mesh& mesh) const {
    if(mesh.mVAO == 0) {
        throw std::runtime_error("Trying to draw deleted Mesh");
    }

    std::size_t numDiffuse = 0;
    std::size_t numSpecular  = 0;

    if(mesh.mColor != std::nullopt) {
        shader.setFloat("color", mesh.mColor->r, mesh.mColor->g, mesh.mColor->b);
    } else {
        shader.setFloat("color", 1.0f, 1.0f, 1.0f);
    }

    bool usesTexture = mesh.mTextureIndices.size() >= 1;
    shader.setBool("usesTexture", usesTexture);

    for(std::size_t i = 0;i < mesh.mTextureIndices.size();i++) {
        const Texture& texture = mTextures[mesh.mTextureIndices[i]];
        texture.bind(GL_TEXTURE0 + i);
        const std::string materialName = "material";
        switch(texture.getType()) {
            case Texture::Type::Diffuse:
                shader.setInt(materialName + std::to_string(numDiffuse++) + ".diffuse", i);
                break;
            case Texture::Type::Specular:
                shader.setInt(materialName + std::to_string(numSpecular++) + ".specular", i);
                break;
        }
    }
    
    glBindVertexArray(mesh.mVAO);
    glDrawElements(GL_TRIANGLES, mesh.mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

CubeModel::CubeModel(unsigned int VAO, unsigned int VBO, const Color& color) : 
    mVAO{VAO},
    mVBO{VBO},
    mColor{color} {}

void CubeModel::draw(const ShaderProgram &shader) const {
    shader.setFloat("color", mColor.r, mColor.g, mColor.b);
    shader.setBool("usesTexture", false);
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

DrawableComponent::DrawableComponent(std::unique_ptr<IDrawable>&& drawable) :
    mDrawable{std::move(drawable)} {}

void DrawableComponent::draw(const ShaderProgram& shader) const {
    mDrawable->draw(shader);
}
