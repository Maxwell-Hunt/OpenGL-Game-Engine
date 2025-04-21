#include "Physics.h"

#include "Transform.h"

#include <iostream>

void PhysicsSystem::run(ECS& ecs, float deltaTime) {
    for(EntityId entity = 0;entity < ecs.numEntities();entity++) {
        if(ecs.hasComponents<Transform, PhysicsComponent>(entity)) {
            Transform& transform = ecs.getComponent<Transform>(entity);
            PhysicsComponent& phys = ecs.getComponent<PhysicsComponent>(entity);
            updatePhys(transform, phys, deltaTime);
        }
    }
}

void PhysicsSystem::updatePhys(Transform& transform, PhysicsComponent& phys, float deltaTime) const {
    transform.position += phys.velocity * deltaTime;
    phys.velocity += phys.acceleration * deltaTime;
}

void PhysicsSystem::onCollision(ECS& ecs, EntityId entityA, EntityId entityB) {
    if(ecs.hasComponents<PhysicsComponent>(entityA)) {
        std::cout << "Physics collision detected\n";
        PhysicsComponent& phys = ecs.getComponent<PhysicsComponent>(entityA);
        phys.velocity.y *= -1;
    }
}