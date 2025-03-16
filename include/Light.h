#ifndef __LIGHT__
#define __LIGHT__

#include <glm/glm.hpp>
#include <ranges>
#include <concepts>

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

template <typename T>
concept PointLightRange =
    std::ranges::range<T> &&
    std::same_as<std::ranges::range_value_t<T>, PointLight>;


#endif