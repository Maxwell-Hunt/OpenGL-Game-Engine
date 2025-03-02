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
#include "Texture.h"
#include "Camera.h"
#include "InputHandler.h"

void handleExit(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void handleMovement(GLFWwindow* window, Camera& camera, double deltaTime) {
    float speed = 10 * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position += camera.forward * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= camera.forward * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position += camera.getRight() * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position -= camera.getRight() * speed;
    }
}

class MouseHandler {
public:
    Camera* camera;
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;

    static MouseHandler handler;

    MouseHandler(Camera* camera) : camera{camera} {}

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        // if (handler.firstMouse)
        // {
        //     handler.lastX = xpos;
        //     handler.lastY = ypos;
        //     handler.firstMouse = false;
        // }
    
        float xoffset = xpos - handler.lastX;
        float yoffset = handler.lastY - ypos; 
        handler.lastX = xpos;
        handler.lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        handler.camera->yaw   += xoffset;
        handler.camera->pitch += yoffset;

        if(handler.camera->pitch > 89.0f)
            handler.camera->pitch = 89.0f;
        if(handler.camera->pitch < -89.0f)
            handler.camera->pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(handler.camera->yaw)) * cos(glm::radians(handler.camera->pitch));
        direction.y = sin(glm::radians(handler.camera->pitch));
        direction.z = sin(glm::radians(handler.camera->yaw)) * cos(glm::radians(handler.camera->pitch));
        handler.camera->forward = glm::normalize(direction);
    }  
};

MouseHandler MouseHandler::handler(NULL);

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void openGlLogic(GLFWwindow* window) {
    Shader vertexShader("/home/maxwell/OpenGLProject/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader fragmentShader("/home/maxwell/OpenGLProject/shaders/frag.glsl", GL_FRAGMENT_SHADER);

    ShaderProgram shaderProgram;
    shaderProgram.attach(std::move(vertexShader)).attach(std::move(fragmentShader)).link();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    Texture crateTexture = Texture("../assets/container.jpg", GL_RGB);
    Texture faceTexture = Texture("../assets/face.png", GL_RGBA);

    // Initialize vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Initialize vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Bind our vertex buffer object to the GL_ARRAY_BUFFER target
    // and copy vertex data into the vertex buffer memory on the GPU associated with the VBO object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify how our vertex attributes are layed out
    unsigned int stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind our Vertex Array Object
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    Camera camera;
    camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.yaw = -90.f;
    camera.pitch = 0.0f;

    CameraController cameraController(camera);

    glfwSetKeyCallback(window, InputHandler::keyCallback);
    glfwSetCursorPosCallback(window, InputHandler::mouseMoveCallback);

    double prevTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double deltaTime = time - prevTime;
        prevTime = time;
        handleExit(window);
        // handleMovement(window, camera, deltaTime);

        cameraController.update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the compiled shader program
        shaderProgram.use();
        crateTexture.bind(GL_TEXTURE0);
        faceTexture.bind(GL_TEXTURE1);

        shaderProgram.setInt("crateTexture", 0);
        shaderProgram.setInt("faceTexture", 1);

        
        glm::mat4 view = camera.view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        shaderProgram.setMat4("view", glm::value_ptr(view));
        shaderProgram.setMat4("projection", glm::value_ptr(projection));

        for(int i = 0;i < 10;i++) {
            glm::mat4 model(1.0f);
            
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(25.0f) * (i+1), glm::vec3(0.5f, 1.0f, 0.0f));

            shaderProgram.setMat4("model", glm::value_ptr(model));
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
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