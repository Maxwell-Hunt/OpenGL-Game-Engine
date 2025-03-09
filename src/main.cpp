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

    Texture containerDiffuseMap("/home/maxwell/OpenGLProject/assets/container2.png");
    Texture containerSpecularMap("/home/maxwell/OpenGLProject/assets/container2_specular.png");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Initialize vertex array object
    unsigned int objectVAO;
    glGenVertexArrays(1, &objectVAO);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);

    // Initialize vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(objectVAO);

    // Bind our vertex buffer object to the GL_ARRAY_BUFFER target
    // and copy vertex data into the vertex buffer memory on the GPU associated with the VBO object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify how our vertex attributes are layed out
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind our Vertex Array Object
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO already contains the data we need
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.yaw = -90.f;
    camera.pitch = 0.0f;

    CameraController cameraController(camera);

    InputManager::init(window);
    glfwSetCursorPosCallback(window, InputManager::mouseMoveCallback);

    double prevTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);

        cameraController.update(deltaTime);

        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 objectModel(1.0f);
        glm::vec3 objectPosition = glm::vec3(0.0f);
        objectModel = glm::translate(objectModel, objectPosition);

        glm::mat4 objectNormalMatrix = glm::transpose(glm::inverse(view * objectModel));

        glm::mat4 lightModel(1.0f);
        float radius = -2.0f;
        float x = cos(glfwGetTime()) * radius;
        float z = sin(glfwGetTime()) * radius;
        glm::vec3 lightPosition = glm::vec3(x, 1.0f, z);
        lightModel = glm::translate(lightModel, lightPosition);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        objectShader.use();

        objectShader.setInt("material.diffuse", 0);
        objectShader.setInt("material.specular", 1);
        containerDiffuseMap.bind(GL_TEXTURE0);
        containerSpecularMap.bind(GL_TEXTURE1);
        objectShader.setFloat("material.shine", 32);

        objectShader.setMat4("view", glm::value_ptr(view));
        objectShader.setMat4("projection", glm::value_ptr(projection));
        objectShader.setMat4("model", glm::value_ptr(objectModel));
        objectShader.setMat4("normalMatrix", glm::value_ptr(objectNormalMatrix));

        glm::vec4 lightViewCoords = view * glm::vec4(lightPosition, 1.0f);
        objectShader.setFloat("light.position", lightViewCoords[0], lightViewCoords[1], lightViewCoords[2]);

        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        objectShader.setFloat("light.color", lightColor[0], lightColor[1], lightColor[2]);
        objectShader.setFloat("light.ambientStrength", 0.2);
        objectShader.setFloat("light.diffuseStrength", 0.5);
        objectShader.setFloat("light.specularStrength", 1.0);
        
        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();

        lightShader.setFloat("lightColor", lightColor[0], lightColor[1], lightColor[2]);

        lightShader.setMat4("view", glm::value_ptr(view));
        lightShader.setMat4("projection", glm::value_ptr(projection));
        lightShader.setMat4("model", glm::value_ptr(lightModel));
         
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightVAO);
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