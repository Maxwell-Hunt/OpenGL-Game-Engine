#ifndef __CAMERA__
#define __CAMERA__

#include <glm/glm.hpp>

#include "InputManager.h"
#include "ECS.h"

class Camera {
public:
    Camera(float aspectRatio);
    glm::mat4 view() const;    
    glm::mat4 perspective() const;
    glm::vec3 getRight() const;

    float yaw;
    float pitch;
    glm::vec3 position;
    glm::vec3 forward;
private:
    float mAspectRatio;
    const static glm::vec3 up;
};

class CameraController : public System {
public:
    CameraController();
    void run(ECS& ecs, float deltaTime);
private:
    void update(float deltaTime, Camera& camera);

    glm::vec3 velocity;
    float speed;

    float lastX;
    float lastY;

    bool firstMouse;
};

#endif