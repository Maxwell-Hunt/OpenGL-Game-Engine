#include "Model.h"

#include <sstream>
#include <iostream>

Texture::Type convertType(aiTextureType type) {
    switch(type) {
        case aiTextureType_DIFFUSE:
            return Texture::Type::Diffuse;
        case aiTextureType_SPECULAR:
            return Texture::Type::Specular;
        default:
            throw std::runtime_error("We don't currently support that texture type");
    }
}

Model::Model(const std::filesystem::path& path) {
    mDirectory = path.parent_path();
    std::cout << "Loading model from: " << path << '\n';
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::ostringstream ss;
        ss << "Failed to load scene " << importer.GetErrorString();
        throw std::runtime_error(ss.str());
    }
    processNode(scene->mRootNode, scene);
}

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
    for(std::size_t i = 0;i < mesh.mTextureIndices.size();i++) {
        const Texture& texture = mTextures[mesh.mTextureIndices[i]];
        texture.bind(GL_TEXTURE0 + i);
        switch(texture.getType()) {
            case Texture::Type::Diffuse:
                shader.setInt("diffuseTexture" + std::to_string(numDiffuse++), i);
                break;
            case Texture::Type::Specular:
                shader.setInt("specularTexture" + std::to_string(numSpecular++), i);
                break;
        }
    }
    glBindVertexArray(mesh.mVAO);
    glDrawElements(GL_TRIANGLES, mesh.mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textureIndices;

    vertices.reserve(mesh->mNumVertices);
    for(unsigned int i = 0;i < mesh->mNumVertices;i++) {
        vertices.push_back(Vertex(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            mesh->mTextureCoords[0] != nullptr 
                ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) 
                : glm::vec2(0.0f, 0.0f)
        ));
    }

    for(unsigned int i = 0;i < mesh->mNumFaces;i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0;j < face.mNumIndices;j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    loadMaterialTextures(material, aiTextureType_DIFFUSE, textureIndices);
    loadMaterialTextures(material, aiTextureType_SPECULAR, textureIndices);
    
    return Mesh(std::move(vertices), std::move(indices), std::move(textureIndices));
}

void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<unsigned int>& textureIndices) {
    unsigned int count = mat->GetTextureCount(type);
    for(unsigned int i = 0;i < count;i++) {
        aiString aiPath;
        mat->GetTexture(type, i, &aiPath);
        std::filesystem::path path = aiPath.C_Str();
        std::optional<unsigned int> index = findTextureIndex(path);
        if(index.has_value()) {
            textureIndices.push_back(index.value());
        } else {
            mTextures.push_back(Texture(mDirectory / path, convertType(type)));
            textureIndices.push_back(mTextures.size()-1);
        }
    }
}

std::optional<unsigned int> Model::findTextureIndex(const std::filesystem::path& path) const {
    std::filesystem::path fullPath = mDirectory / path;
    for(unsigned int i = 0;i < mTextures.size();i++) {
        if(fullPath == mTextures[i].getPath()) {
            return i;
        }
    }
    return std::nullopt;
}