#ifndef __CAMERA__
#define __CAMERA__

#include <glm/glm.hpp>

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

#endif