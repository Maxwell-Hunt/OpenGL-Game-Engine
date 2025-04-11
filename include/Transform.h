#ifndef __TRANSFORM__
#define __TRANSFORM__

#include <glm/glm.hpp>

class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 getModel() const;
};


#endif