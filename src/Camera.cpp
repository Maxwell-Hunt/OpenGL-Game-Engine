#include "Camera.h"

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
    KeyCallbackReceiver(),
    MouseCallbackReceiver(),
    camera{camera},
    velocity{0.0f},
    speed{10.0f},
    lastX{400.0f},
    lastY{300.0f} {}

void CameraController::keyCallback(int key, int action) {
    float pressed;
    if(action == GLFW_PRESS) { pressed = 1.0f; }
    else if(action == GLFW_RELEASE) { pressed = -1.0f; }
    else { return; }
    
    if(key == GLFW_KEY_W) {
        velocity += camera.forward * speed * pressed;
    }
    if(key == GLFW_KEY_S) {
        velocity -= camera.forward * speed * pressed;
    }
    if(key == GLFW_KEY_D) {
        velocity += camera.getRight() * speed * pressed;
    }
    if(key == GLFW_KEY_A) {
        velocity -= camera.getRight() * speed * pressed;
    }
}

void CameraController::mouseCallback(double xPos, double yPos) {
    // if (firstMouse)
        // {
        //     lastX = xpos;
        //     lastY = ypos;
        //     firstMouse = false;
        // }
    
        float xoffset = xPos - lastX;
        float yoffset = lastY - yPos; 
        lastX = xPos;
        lastY = yPos;

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

void CameraController::update(float deltaTime) {
    camera.position += velocity * deltaTime;
}
