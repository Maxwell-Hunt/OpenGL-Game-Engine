#ifndef __CAMERA__
#define __CAMERA__

#include <glm/glm.hpp>

#include "InputHandler.h"

class Camera {
public:
    glm::mat4 view() const;    
    glm::vec3 getRight() const;

    float yaw;
    float pitch;
    glm::vec3 position;
    glm::vec3 forward;
private:
    const static glm::vec3 up;
};

class CameraController : public MouseCallbackReceiver, public KeyCallbackReceiver {
public:
    CameraController(Camera& camera);
    void keyCallback(int key, int action) override;
    void mouseCallback(double xPos, double yPos) override;

    void update(float deltaTime);
private:
    Camera& camera;
    glm::vec3 velocity;
    float speed;

    float lastX;
    float lastY;
};

#endif