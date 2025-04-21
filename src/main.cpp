#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"
#include "Camera.h"
#include "InputManager.h"

#include "Texture.h"
#include "Mesh.h"
#include "ModelFactory.h"

#include "Renderer.h"

void handleExit(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void openGlLogic(GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);

    Camera camera(800.0f / 600.0f);
    CameraController cameraController;

    InputManager::init(window);
    glfwSetCursorPosCallback(window, InputManager::mouseMoveCallback);
    
    std::vector<PointLight> pointLights;
    DirectionalLight skyLight;

    RenderSystem renderer(skyLight, pointLights);

    DrawableComponent cube = CubeModelFactory::createCube({1.0f, 0.0f, 0.0f}, LightingType::NoLighting);
    DrawableComponent backpack = ModelFactory::loadModel("/home/maxwell/OpenGLProject/assets/sphere/sphere.obj");

    ECS ecs;
    ecs.addSystem(&renderer);
    ecs.addSystem(&cameraController);

    EntityId mainCamera = ecs.createEntity();
    EntityId object = ecs.createEntity();
    EntityId light = ecs.createEntity();

    Transform objectTransform = {
        glm::vec3(0.0f),
        glm::vec3(0.0f, glfwGetTime(), 0.0f),
        glm::vec3(1.0f)
    };

    float radius = 6.0f;
    glm::vec3 lightPosition(radius * cos(glfwGetTime()), 0.0f, radius * sin(glfwGetTime()));
    Transform lightTransform  = {lightPosition, glm::vec3(0.0f), glm::vec3(0.5f)};

    ecs.addComponentToEntity(mainCamera, std::move(camera));

    ecs.addComponentToEntity(object, std::move(objectTransform));
    ecs.addComponentToEntity(object, std::move(backpack));

    ecs.addComponentToEntity(light, std::move(cube));
    ecs.addComponentToEntity(light, std::move(lightTransform));

    double prevTime = glfwGetTime();

    Color backgroundColor = {0.203f, 0.203f, 0.203f};
    while(!glfwWindowShouldClose(window)) {
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);
        
        glm::vec3 lightDirection(0.0f, 1.0f, 0.0f);
        glm::vec3 pointLightColor(1.0f, 0.0f, 0.0f);

        skyLight = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            0.1f,
            0.5f,
            0.5f,
            lightDirection
        };

        pointLights = std::vector<PointLight>({{
            pointLightColor,
            0.02f,
            1.0f,
            1.0f,
            lightPosition,
            1.0f,
            0.09f,
            0.032f 
        }});

        ecs.runSystems(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Window", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to Create Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to Initialize Glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    
    openGlLogic(window);

    glfwTerminate();
    return 0;
}