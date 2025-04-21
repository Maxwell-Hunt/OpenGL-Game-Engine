#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include "Light.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>

class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw(const ShaderProgram& shader) const = 0; 
    virtual LightingType getLightingType() const = 0;
};

class Model : public IDrawable {
friend class ModelFactory;
public:
    virtual ~Model() override = default;
    virtual void draw(const ShaderProgram& shader) const override;
    virtual LightingType getLightingType() const override;
private:
    Model(LightingType lightingType, std::filesystem::path&& directory, std::vector<Mesh>&& meshes, std::vector<Texture>&& textures);
    void drawMesh(const ShaderProgram& shader, const Mesh& mesh) const;

    LightingType mLightingType;
    std::filesystem::path mDirectory;
    std::vector<Mesh> mMeshes;
    std::vector<Texture> mTextures;
};

class CubeModel : public IDrawable {
friend class CubeModelFactory;
public:
    virtual ~CubeModel() override = default;
    virtual void draw(const ShaderProgram& shader) const override;
    virtual LightingType getLightingType() const override;
private:
    CubeModel(LightingType lightingType, unsigned int VAO, unsigned int VBO, const Color& color);

    LightingType mLightingType;
    unsigned int mVAO;
    unsigned int mVBO;
    Color mColor;
};

class DrawableComponent : public IDrawable {
public:
    DrawableComponent(std::unique_ptr<IDrawable>&& drawable);
    virtual ~DrawableComponent() override = default;
    DrawableComponent(const DrawableComponent& other) = delete;
    DrawableComponent(DrawableComponent&& other) = default;
    DrawableComponent& operator=(const DrawableComponent& other) = delete;
    DrawableComponent& operator=(DrawableComponent&& other) = default;
    virtual void draw(const ShaderProgram& shader) const override;
    virtual LightingType getLightingType() const override;
private:
    std::unique_ptr<IDrawable> mDrawable;
};

#endif