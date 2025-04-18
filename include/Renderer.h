#ifndef __RENDERER__
#define __RENDERER__

#include "Shader.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "ECS.h"

class RenderSystem : public System {
public:
    RenderSystem(const Camera& camera, const DirectionalLight& skyLight, const std::vector<PointLight>& pointLights);
    virtual ~RenderSystem() override = default;
    virtual void run(ECS& ecs) override;
private:
    void renderNoLighting(const Transform& transform, const IDrawable& drawable) const;
    void renderPhong(const Transform& transform, const IDrawable& drawable) const;

    const Camera& mCamera;
    const DirectionalLight& mSkyLight;
    const std::vector<PointLight>& mPointLights;
    ShaderProgram mPhongShader;
    ShaderProgram mNoLightShader;
};

#endif