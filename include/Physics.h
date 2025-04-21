#ifndef __PHYSICS__
#define __PHYSICS__

#include "ECS.h"
#include "Transform.h"
#include <glm/glm.hpp>

struct PhysicsComponent {
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

class PhysicsSystem : public System {
public:
    virtual ~PhysicsSystem() override = default;
    virtual void run(ECS& ecs, float deltaTime) override; 
private:
    void updatePhys(Transform& transform, PhysicsComponent& phys, float deltaTime);
};

#endif