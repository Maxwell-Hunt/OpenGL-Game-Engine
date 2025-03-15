#ifndef __LIGHT__
#define __LIGHT__

#include <glm/glm.hpp>

struct Light {
    glm::vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

struct DirectionalLight : public Light {
    glm::vec3 direction;
};

struct PointLight : public Light {
    glm::vec3 position;
    float kc;
    float kl;
    float kq;
};

#endif