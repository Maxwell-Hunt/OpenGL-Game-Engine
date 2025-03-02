#include "InputManager.h"

GLFWwindow* InputManager::window = nullptr;
glm::vec2 InputManager::mousePosition = glm::vec2(0.0f);

void InputManager::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    mousePosition[0] = xpos;
    mousePosition[1] = ypos;
}

bool InputManager::isPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

const glm::vec2& InputManager::getMousePosition() {
    return mousePosition;
}

void InputManager::init(GLFWwindow *window) {
    InputManager::window = window;
}
