#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>

class Model {
friend class ModelFactory;
public:
    void draw(const ShaderProgram& shader) const;
private:
    Model(const std::filesystem::path& path);
    void drawMesh(const ShaderProgram& shader, const Mesh& mesh) const;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<unsigned int>& textureIndices);
    std::optional<unsigned int> findTextureIndex(const std::filesystem::path& path) const;

    std::filesystem::path mDirectory;
    std::vector<Mesh> mMeshes;
    std::vector<Texture> mTextures;
};

class ModelFactory {
public:
    static Model loadModel(const std::filesystem::path& path);
};

#endif