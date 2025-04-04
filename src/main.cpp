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
#include "Model.h"

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

    ShaderProgram objectShader;
    objectShader.attach(std::move(objectVertexShader)).attach(std::move(objectFragmentShader)).link();
    
    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.yaw = -90.f;
    camera.pitch = 0.0f;

    CameraController cameraController(camera);

    InputManager::init(window);
    glfwSetCursorPosCallback(window, InputManager::mouseMoveCallback);

    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
        Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f))
    };
    std::vector<unsigned int> indices = {0, 1, 2};
    std::vector<unsigned int> textures;
    
    Model backpack = ModelFactory::loadModel("/home/maxwell/OpenGLProject/assets/backpack/backpack.obj");
    std::cout << "Loading Complete" << std::endl;

    double prevTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);

        cameraController.update(deltaTime);

        glm::mat4 model(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * 0.2f, glm::vec3(0.2f, 0.3f, 0.5f));
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 modelViewMatrix = view * model;
        glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

        glm::vec3 lightPosition(3.f * cos(glfwGetTime()), 0.0f, 3.f * sin(glfwGetTime()));

        objectShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
        objectShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
        objectShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));

        objectShader.setInt("numLights", 1);
        objectShader.setDirectionalLight("skyLight", {
            glm::vec3(0.0f, 1.0f, 0.0f),
            0.5f,
            0.3f,
            0.3f,
            glm::vec3(0.2f, 0.3f, -0.7f)
        });

        objectShader.setPointLights("pointLights", std::array<PointLight, 1>({{
            glm::vec3(1.0f, 0.0f, 0.0f),
            0.0f,
            0.8f,
            0.8f,
            lightPosition,
            1.0f,
            0.09f,
            0.032f
        }}));
        objectShader.setFloat("material0.shine", 32.0f);

        objectShader.use();
        backpack.draw(objectShader);

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