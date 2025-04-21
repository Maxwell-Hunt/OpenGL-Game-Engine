#ifndef __RENDERER__
#define __RENDERER__

#include "Shader.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "ECS.h"

class RenderSystem : public System {
public:
    RenderSystem();
    virtual ~RenderSystem() override = default;
    virtual void run(ECS& ecs, float deltaTime) override;
private:
    void renderNoLighting(const Transform& transform, const IDrawable& drawable, const Camera& camera) const;
    void renderPhong(const Transform& transform, const IDrawable& drawable, const Camera& camera, const DirectionalLight& skyLight, const std::vector<PointLightComponents>& pointLights) const;

    ShaderProgram mPhongShader;
    ShaderProgram mNoLightShader;
};

#endif