#ifndef __INPUTHANDLER__
#define __INPUTHANDLER__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager {
public:
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static bool isPressed(int key);
    static const glm::vec2& getMousePosition();
    static void init(GLFWwindow* window);
private:
    static GLFWwindow* window;
    static glm::vec2 mousePosition;
};

#endif
