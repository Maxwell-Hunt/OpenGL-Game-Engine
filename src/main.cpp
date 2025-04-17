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
    Shader objectVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader objectFragmentShader("/home/maxwell/OpenGLProject/shaders/objFrag.glsl", GL_FRAGMENT_SHADER);

    Shader lightVertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader lightFragmentShader("/home/maxwell/OpenGLProject/shaders/lightFrag.glsl", GL_FRAGMENT_SHADER);

    ShaderProgram objectShader;
    objectShader.attach(std::move(objectVertexShader)).attach(std::move(objectFragmentShader)).link();
    ShaderProgram lightShader;
    lightShader.attach(std::move(lightVertexShader)).attach(std::move(lightFragmentShader)).link();

    
    glEnable(GL_DEPTH_TEST);

    Camera camera(800.0f / 600.0f);
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.yaw = -90.f;
    camera.pitch = 0.0f;

    CameraController cameraController(camera);

    InputManager::init(window);
    glfwSetCursorPosCallback(window, InputManager::mouseMoveCallback);
    
    std::vector<PointLight> pointLights;
    DirectionalLight skyLight;

    RenderSystem renderer(camera, objectShader, skyLight, pointLights);

    DrawableComponent cube = CubeModelFactory::createCube({1.0f, 0.0f, 0.0f});
    DrawableComponent backpack = ModelFactory::loadModel("/home/maxwell/OpenGLProject/assets/backpack/backpack.obj");

    ECS ecs;
    EntityId object = ecs.createEntity();
    EntityId light = ecs.createEntity();

    Transform objectTransform = {
        glm::vec3(0.0f),
        glm::vec3(0.0f, glfwGetTime(), 0.0f),
        glm::vec3(1.0f)
    };

    float radius = 6.0f;
    glm::vec3 lightPosition(radius * cos(glfwGetTime()), 0.0f, radius * sin(glfwGetTime()));
    Transform lightTransform  = {lightPosition, glm::vec3(0.0f), glm::vec3(1.0f)};

    ecs.addComponentToEntity(object, &objectTransform);
    ecs.addComponentToEntity(object, &backpack);

    ecs.addComponentToEntity(light, &cube);
    ecs.addComponentToEntity(light, &lightTransform);

    double prevTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);

        cameraController.update(deltaTime);

        objectTransform = {
            glm::vec3(0.0f),
            glm::vec3(0.0f, glfwGetTime(), 0.0f),
            glm::vec3(1.0f)
        };

        lightPosition = glm::vec3(radius * cos(glfwGetTime()), 0.0f, radius * sin(glfwGetTime()));
        lightTransform  = {lightPosition, glm::vec3(0.0f), glm::vec3(1.0f)};
        
        glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
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

        renderer.run(ecs);
        
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