#include "Physics.h"

#include "Transform.h"

void PhysicsSystem::run(ECS& ecs, float deltaTime) {
    for(EntityId entity = 0;entity < ecs.numEntities();entity++) {
        if(ecs.hasComponents<Transform, PhysicsComponent>(entity)) {
            Transform& transform = ecs.getComponent<Transform>(entity);
            PhysicsComponent& phys = ecs.getComponent<PhysicsComponent>(entity);
            updatePhys(transform, phys, deltaTime);
        }
    }
}

void PhysicsSystem::updatePhys(Transform& transform, PhysicsComponent& phys, float deltaTime) {
    transform.position += phys.velocity * deltaTime;
    phys.velocity += phys.acceleration * deltaTime;
}