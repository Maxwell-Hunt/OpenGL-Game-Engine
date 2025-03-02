#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 Camera::view() const {
    return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(forward, up));
}