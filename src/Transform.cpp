#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 Transform::getModel() const {
    glm::mat4 result(1.0f);
    result = glm::scale(result, scale);
    result = glm::rotate(result, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    result = glm::rotate(result, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    result = glm::rotate(result, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    result = glm::translate(result, position);
    return result;
}