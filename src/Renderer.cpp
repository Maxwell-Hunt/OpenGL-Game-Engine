#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

RenderSystem::RenderSystem(const Camera& camera, const DirectionalLight& skyLight, const std::vector<PointLight>& pointLights) :
    mCamera{camera},
    mSkyLight{skyLight},
    mPointLights{pointLights} {
        Shader phongVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
        Shader phongFragmentShader("/home/maxwell/OpenGLProject/shaders/objFrag.glsl", GL_FRAGMENT_SHADER);
        mPhongShader.attach(std::move(phongVertexShader)).attach(std::move(phongFragmentShader)).link();

        Shader noLightingVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
        Shader noLightingFragmentShader("/home/maxwell/OpenGLProject/shaders/simplefrag.glsl", GL_FRAGMENT_SHADER);
        mNoLightShader.attach(std::move(noLightingVertexShader)).attach(std::move(noLightingFragmentShader)).link();
    }

void RenderSystem::run(ECS& ecs) {
    std::vector<EntityId> noLightingObjects;
    std::vector<EntityId> phongShadedObjects;
    for(EntityId entity = 0;entity < ecs.numEntities();entity++) {
        if(ecs.hasComponents<Transform, DrawableComponent>(entity)) {
            DrawableComponent* model = ecs.getComponent<DrawableComponent>(entity);
            switch(model->getLightingType()) {
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
    }

    mPhongShader.use();
    for(EntityId entity : phongShadedObjects) {
        Transform* transform = ecs.getComponent<Transform>(entity);
        DrawableComponent* model = ecs.getComponent<DrawableComponent>(entity);
        renderPhong(*transform, *model);
    }

    mNoLightShader.use();
    for(EntityId entity : noLightingObjects) {
        Transform* transform = ecs.getComponent<Transform>(entity);
        DrawableComponent* model = ecs.getComponent<DrawableComponent>(entity);
        renderNoLighting(*transform, *model);
    }
}

void RenderSystem::renderNoLighting(const Transform& transform, const IDrawable& drawable) const {
    glm::mat4 model = transform.getModel();
    glm::mat4 view = mCamera.view();
    glm::mat4 projection = mCamera.perspective();

    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    mNoLightShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
    mNoLightShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
    mNoLightShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));

    drawable.draw(mNoLightShader);
}

void RenderSystem::renderPhong(const Transform& transform, const IDrawable& drawable) const {
    glm::mat4 model = transform.getModel();
    glm::mat4 view = mCamera.view();
    glm::mat4 projection = mCamera.perspective();

    glm::mat4 modelViewMatrix = view * model;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    mPhongShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
    mPhongShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
    mPhongShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));
    
    mPhongShader.setInt("numLights", mPointLights.size());

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


    mPhongShader.setDirectionalLight("skyLight", viewSpaceDirectionTransformer(mSkyLight));
    mPhongShader.setPointLights("pointLights", viewSpacePointTransformer(mPointLights));

    mPhongShader.setFloat("material0.shine", 32.0f);
    
    drawable.draw(mPhongShader);
}