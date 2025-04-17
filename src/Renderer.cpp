#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

RenderSystem::RenderSystem(const Camera& camera, const ShaderProgram& shader, const DirectionalLight& skyLight, const std::vector<PointLight>& pointLights) :
    mCamera{camera},
    mShader{shader},
    mSkyLight{skyLight},
    mPointLights{pointLights} {}

void RenderSystem::run(ECS& ecs) {
    for(EntityId entity = 0;entity < ecs.numEntities();entity++) {
        if(ecs.hasComponents<Transform, DrawableComponent>(entity)) {
            Transform* transform = ecs.getComponent<Transform>(entity);
            DrawableComponent* model = ecs.getComponent<DrawableComponent>(entity);
            render(*transform, *model);
        }
    }
}

void RenderSystem::render(const Transform& transform, const IDrawable& drawable) const {
    glm::mat4 model = transform.getModel();
    glm::mat4 view = mCamera.view();
    glm::mat4 projection = mCamera.perspective();

    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    mShader.use();
    mShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
    mShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
    mShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));
    
    mShader.setInt("numLights", mPointLights.size());

    auto viewSpaceDirectionTransformer = [view](DirectionalLight light) {
        light.direction = glm::normalize(glm::mat3(view) * light.direction);
        return light;
    };

    auto viewSpacePointTransformer = [view](std::vector<PointLight> pointLights) {
        for(PointLight& light : pointLights) {
            light.position = glm::vec3(view * glm::vec4(light.position, 1.0f));
        }
        return pointLights;
    };

    DirectionalLight viewSpaceSkyLight = mSkyLight;
    viewSpaceSkyLight.direction = glm::normalize(glm::mat3(view) * mSkyLight.direction);


    mShader.setDirectionalLight("skyLight", viewSpaceDirectionTransformer(mSkyLight));
    mShader.setPointLights("pointLights", viewSpacePointTransformer(mPointLights));

    mShader.setFloat("material0.shine", 32.0f);
    
    drawable.draw(mShader);
}