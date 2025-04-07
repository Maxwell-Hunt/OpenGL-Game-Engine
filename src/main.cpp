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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
    
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
    
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
    
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
    
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    
    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.yaw = -90.f;
    camera.pitch = 0.0f;

    CameraController cameraController(camera);

    InputManager::init(window);
    glfwSetCursorPosCallback(window, InputManager::mouseMoveCallback);
    
    Model backpack = ModelFactory::loadModel("/home/maxwell/OpenGLProject/assets/backpack/backpack.obj");

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
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 modelViewMatrix = view * model;
        glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

        float radius = 5.0f;
        glm::vec3 lightPosition(radius * cos(glfwGetTime()), 0.0f, radius * sin(glfwGetTime()));
        glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
        glm::vec3 pointLightColor(1.0f, 0.0f, 0.0f);
        
        objectShader.use();
        objectShader.setMat4("modelViewMatrix", glm::value_ptr(modelViewMatrix));
        objectShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(modelViewProjectionMatrix));
        objectShader.setMat4("normalMatrix", glm::value_ptr(normalMatrix));

        objectShader.setInt("numLights", 1);
        objectShader.setDirectionalLight("skyLight", {
            glm::vec3(1.0f, 1.0f, 1.0f),
            0.1f,
            0.5f,
            0.5f,
            glm::normalize(glm::mat3(view) * lightDirection)
        });

        objectShader.setPointLights("pointLights", std::array<PointLight, 1>({{
            pointLightColor,
            0.02f,
            1.0f,
            1.0f,
            glm::vec3(view * glm::vec4(lightPosition, 1.0f)),
            1.0f,
            0.09f,
            0.032f
        }}));

        objectShader.setFloat("material0.shine", 32.0f);

        backpack.draw(objectShader);

        glm::mat4 lightModel(1.0f);
        lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));
        lightModel = glm::translate(lightModel, lightPosition);
        glm::mat4 lightModelViewProjection = projection * view * lightModel;
        lightShader.use();
        lightShader.setMat4("modelViewProjectionMatrix", glm::value_ptr(lightModelViewProjection));
        lightShader.setFloat("lightColor", pointLightColor[0], pointLightColor[1], pointLightColor[2]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

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