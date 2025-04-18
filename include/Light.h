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

struct Color {
public:
    float r, g, b;
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

enum class LightingType {
    NoLighting,
    Flat,
    Phong
};

template <typename T>
concept PointLightRange =
    std::ranges::range<T> &&
    std::same_as<std::ranges::range_value_t<T>, PointLight>;


#endif