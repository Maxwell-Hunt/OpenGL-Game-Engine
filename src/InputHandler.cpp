#include "InputHandler.h"

KeyCallbackReceiver::KeyCallbackReceiver() {
    InputHandler::addKeyCallback(this);
}

KeyCallbackReceiver::~KeyCallbackReceiver() {
    InputHandler::removeKeyCallback(this);
}

MouseCallbackReceiver::MouseCallbackReceiver() {
    InputHandler::addMouseCallback(this);
}

MouseCallbackReceiver::~MouseCallbackReceiver() {
    InputHandler::removeMouseCallback(this);
}

InputHandler::InputHandler() {}

InputHandler InputHandler::handler = InputHandler::createHandler();

InputHandler InputHandler::createHandler() {
    return InputHandler();
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    handler.notifyKeySubscribers(key, action);
}

void InputHandler::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    handler.notifyMouseSubscribers(xpos, ypos);
}

void InputHandler::addKeyCallback(KeyCallbackReceiver* callback) {
    handler.keyCallbacks.insert(callback);
}

void InputHandler::addMouseCallback(MouseCallbackReceiver* callback) {
    handler.mouseCallbacks.insert(callback);
}

void InputHandler::removeKeyCallback(KeyCallbackReceiver* callback) {
    handler.keyCallbacks.erase(callback);
}

void InputHandler::removeMouseCallback(MouseCallbackReceiver* callback) {
    handler.mouseCallbacks.erase(callback);
}

void InputHandler::notifyKeySubscribers(int key, int action) const {
    for(KeyCallbackReceiver* receiver : keyCallbacks) {
        receiver->keyCallback(key, action);
    }
}

void InputHandler::notifyMouseSubscribers(double xpos, double ypos) const {
    for(MouseCallbackReceiver* receiver : mouseCallbacks) {
        receiver->mouseCallback(xpos, ypos);
    }
}
