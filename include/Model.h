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
    Model(std::filesystem::path&& directory, std::vector<Mesh>&& meshes, std::vector<Texture>&& textures);
    void drawMesh(const ShaderProgram& shader, const Mesh& mesh) const;

    std::filesystem::path mDirectory;
    std::vector<Mesh> mMeshes;
    std::vector<Texture> mTextures;
};

class ModelFactory {
public:
    static Model loadModel(const std::filesystem::path& path);
};

#endif