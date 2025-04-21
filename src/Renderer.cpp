#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

RenderSystem::RenderSystem() {
    Shader phongVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader phongFragmentShader("/home/maxwell/OpenGLProject/shaders/objFrag.glsl", GL_FRAGMENT_SHADER);
    mPhongShader.attach(std::move(phongVertexShader)).attach(std::move(phongFragmentShader)).link();

    Shader noLightingVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader noLightingFragmentShader("/home/maxwell/OpenGLProject/shaders/simplefrag.glsl", GL_FRAGMENT_SHADER);
    mNoLightShader.attach(std::move(noLightingVertexShader)).attach(std::move(noLightingFragmentShader)).link();
}

void RenderSystem::run(ECS& ecs, float deltaTime) {
    std::vector<EntityId> noLightingObjects;
    std::vector<EntityId> phongShadedObjects;

    std::vector<EntityId> pointLightsEntities;
    EntityId skyLightEntity = 0;
    EntityId cameraEntity = 0;

    std::vector<PointLightComponents> pointLightComponents;
    for(EntityId entity = 0;entity < ecs.numEntities();entity++) {
        if(ecs.hasComponents<Transform, DrawableComponent>(entity)) {
            DrawableComponent& model = ecs.getComponent<DrawableComponent>(entity);
            switch(model.getLightingType()) {
                case LightingType::NoLighting:
                    noLightingObjects.push_back(entity);
                    break;
                case LightingType::Phong:
                    phongShadedObjects.push_back(entity);
                    break;
                default:
                    noLightingObjects.push_back(entity);
                    break;
            }
        }

        if(ecs.hasComponents<Camera>(entity)) {
            cameraEntity = entity;
        }

        if(ecs.hasComponents<PointLight>(entity) && ecs.hasComponents<Transform>(entity)) {
            PointLight& pointLight = ecs.getComponent<PointLight>(entity);
            Transform& transform = ecs.getComponent<Transform>(entity);
            pointLightComponents.push_back({pointLight, transform});
        }

        if(ecs.hasComponents<DirectionalLight>(entity)) {
            skyLightEntity = entity;
        }
    }

    Camera& camera = ecs.getComponent<Camera>(cameraEntity);
    DirectionalLight& skyLight = ecs.getComponent<DirectionalLight>(skyLightEntity);
    mPhongShader.use();
    for(EntityId entity : phongShadedObjects) {
        Transform& transform = ecs.getComponent<Transform>(entity);
        DrawableComponent& model = ecs.getComponent<DrawableComponent>(entity);

        renderPhong(transform, model, camera, skyLight, pointLightComponents);
    }

    mNoLightShader.use();
    for(EntityId entity : noLightingObjects) {
        Transform& transform = ecs.getComponent<Transform>(entity);
        DrawableComponent& model = ecs.getComponent<DrawableComponent>(entity);
        renderNoLighting(transform, model, camera);
    }
}

void RenderSystem::renderNoLighting(const Transform& transform, const IDrawable& drawable, const Camera& camera) const {
    glm::mat4 model = transform.getModel();
    glm::mat4 view = camera.view();
    glm::mat4 projection = camera.perspective();

    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    mNoLightShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
    mNoLightShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
    mNoLightShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));

    drawable.draw(mNoLightShader);
}

void RenderSystem::renderPhong(const Transform& transform, const IDrawable& drawable, const Camera& camera, const DirectionalLight& skyLight, const std::vector<PointLightComponents>& pointLights) const {
    glm::mat4 model = transform.getModel();
    glm::mat4 view = camera.view();
    glm::mat4 projection = camera.perspective();

    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    mPhongShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
    mPhongShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
    mPhongShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));
    
    mPhongShader.setInt("numLights", pointLights.size());

    auto viewSpaceDirectionTransformer = [view](DirectionalLight light) {
        light.direction = glm::normalize(glm::mat3(view) * light.direction);
        return light;
    };

    auto viewSpacePointTransformer = [view](std::vector<PointLightComponents> pointLights) {
        for(PointLightComponents& light : pointLights) {
            light.transform.position = glm::vec3(view * glm::vec4(light.transform.position, 1.0f));
        }
        return pointLights;
    };

    mPhongShader.setDirectionalLight("skyLight", viewSpaceDirectionTransformer(skyLight));
    mPhongShader.setPointLights("pointLights", pointLights);
    mPhongShader.setPointLights("pointLights", viewSpacePointTransformer(pointLights));

    mPhongShader.setFloat("material0.shine", 32.0f);
    
    drawable.draw(mPhongShader);
}