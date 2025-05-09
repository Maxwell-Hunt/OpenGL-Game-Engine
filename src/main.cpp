#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Camera.h"
#include "InputManager.h"

#include "ModelFactory.h"

#include "Renderer.h"
#include "Physics.h"

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
    
    RenderSystem renderer;
    PhysicsSystem physics;
    CollisionSystem collisions;

    collisions.registerCallback(&physics);

    ECS ecs;
    ecs.addSystem(&renderer);
    ecs.addSystem(&cameraController);
    ecs.addSystem(&physics);
    ecs.addSystem(&collisions);
    
    double prevTime = glfwGetTime();

    EntityId mainCamera = ecs.createEntity();
    ecs.addComponentToEntity(mainCamera, std::move(camera));

    EntityId mainBall = ecs.createEntity();
    ecs.addComponentToEntity(mainBall, ModelFactory::loadModel("/home/maxwell/OpenGLProject/assets/sphere/sphere.obj"));
    ecs.addComponentToEntity(mainBall, Transform(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
    ecs.addComponentToEntity(mainBall, PhysicsComponent(glm::vec3(0.0f), glm::vec3(0.0f, -3.5f, 0.0f)));
    ecs.addComponentToEntity(mainBall, SphereCollider(1.0f));

    EntityId skyLight = ecs.createEntity();
    ecs.addComponentToEntity(skyLight, DirectionalLight(Light(glm::vec3(1.0f), 0.5f, 0.7f, 0.7f), glm::vec3(1.0f, -1.0f, 0.0f)));

    EntityId platform = ecs.createEntity();
    ecs.addComponentToEntity(platform, CubeModelFactory::createCube({1.0f, 0.0f, 0.0f}));
    ecs.addComponentToEntity(platform, Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(10.0f, 1.0f, 10.0f)));
    ecs.addComponentToEntity(platform, XZPlaneCollider(-1.5f));

    Color backgroundColor = {0.203f, 0.203f, 0.203f};
    while(!glfwWindowShouldClose(window)) {
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);
        
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