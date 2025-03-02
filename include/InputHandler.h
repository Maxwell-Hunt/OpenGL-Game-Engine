#ifndef __INPUTHANDLER__
#define __INPUTHANDLER__

#include <GLFW/glfw3.h>
#include <set>

class KeyCallbackReceiver {
public:
    KeyCallbackReceiver();
    virtual ~KeyCallbackReceiver();
    virtual void keyCallback(int key, int action) = 0;
};

class MouseCallbackReceiver {
public:
    MouseCallbackReceiver();
    virtual ~MouseCallbackReceiver();
    virtual void mouseCallback(double xpos, double ypos) = 0;
};

class InputHandler {
public:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

    static void addKeyCallback(KeyCallbackReceiver* callback);
    static void addMouseCallback(MouseCallbackReceiver* callback);

    static void removeKeyCallback(KeyCallbackReceiver* callback);
    static void removeMouseCallback(MouseCallbackReceiver* callback);

    
private:
    InputHandler();
    static InputHandler createHandler();

    void notifyKeySubscribers(int key, int action) const;
    void notifyMouseSubscribers(double xpos, double ypos) const;

    static InputHandler handler;

    std::set<KeyCallbackReceiver*> keyCallbacks;
    std::set<MouseCallbackReceiver*> mouseCallbacks; 
};

#endif
