#include "Camera.h"
#include "InputManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 Camera::view() const {
    return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(forward, up));
}

CameraController::CameraController(Camera &camera) :
    camera{camera},
    velocity{0.0f},
    speed{10.0f},
    lastX{400.0f},
    lastY{300.0f},
    firstMouse{true} {}

void CameraController::update(float deltaTime) {
    if(InputManager::isPressed(GLFW_KEY_W)) {
        camera.position += camera.forward * speed * deltaTime;
    }
    if(InputManager::isPressed(GLFW_KEY_S)) {
        camera.position -= camera.forward * speed * deltaTime;
    }
    if(InputManager::isPressed(GLFW_KEY_D)) {
        camera.position += camera.getRight() * speed * deltaTime;
    }
    if(InputManager::isPressed(GLFW_KEY_A)) {
        camera.position -= camera.getRight() * speed * deltaTime;
    }

    float mouseX = InputManager::getMousePosition()[0];
    float mouseY = InputManager::getMousePosition()[1];

    if (firstMouse)
    {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }
    
    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY; 
    lastX = mouseX;
    lastY = mouseY;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw   += xoffset;
    camera.pitch += yoffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.forward = glm::normalize(direction);
}
