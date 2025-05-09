#include "Collision.h"

#include "Transform.h"

void CollisionSystem::run(ECS& ecs, float deltaTime) {
    for(EntityId entityA = 0;entityA < ecs.numEntities();entityA++) {
        if(!ecs.hasComponents<SphereCollider, Transform>(entityA)) continue;
        for(EntityId entityB = 0;entityB < ecs.numEntities();entityB++) {
            if(entityA == entityB) continue;
            if(ecs.hasComponents<XZPlaneCollider>(entityB)) {
                Transform& sphereTransform = ecs.getComponent<Transform>(entityA);
                SphereCollider& sphereCollider = ecs.getComponent<SphereCollider>(entityA);
                XZPlaneCollider& planeCollider = ecs.getComponent<XZPlaneCollider>(entityB);
                if(planeCollider.y >= sphereTransform.position.y - sphereCollider.radius * sphereTransform.scale.y &&
                    planeCollider.y <= sphereTransform.position.y + sphereCollider.radius * sphereTransform.scale.y) {
                         notifyCallbacks(ecs, entityA, entityB);
                }
            }
        }
    }
}

void CollisionSystem::registerCallback(CollisionSystem::Callback* callback) {
    mCallbacks.insert(callback);
}

void CollisionSystem::unRegisterCallback(CollisionSystem::Callback* callback) {
    mCallbacks.erase(callback);
}

void CollisionSystem::notifyCallbacks(ECS& ecs, EntityId entityA, EntityId entityB) {
    for(CollisionSystem::Callback* callback : mCallbacks) {
        callback->onCollision(ecs, entityA, entityB);
    }
}