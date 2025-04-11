#ifndef __RENDERER__
#define __RENDERER__

#include "Shader.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(const Camera& camera, const ShaderProgram& shader, const DirectionalLight& skyLight, const std::vector<PointLight>& pointLights);
    void render(const Transform& transform, const IDrawable& drawable) const;
private:
    const Camera& mCamera;
    const ShaderProgram& mShader;
    const DirectionalLight& mSkyLight;
    const std::vector<PointLight>& mPointLights;
};

#endif