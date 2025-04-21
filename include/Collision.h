#ifndef __COLLISION__
#define __COLLISION__

#include "ECS.h"

struct SphereCollider {
    float radius;
};

struct XZPlaneCollider {
    float y;
};

class CollisionSystem : public System {
public:
    virtual ~CollisionSystem() override = default;
    virtual void run(ECS& ecs, float deltaTime);

    class Callback {
    public:
        friend class CollisionSystem;
        Callback() = default;
        virtual ~Callback() = default;
    private:
        virtual void onCollision(ECS& ecs, EntityId entityA, EntityId entityB) = 0;
    };

    void registerCallback(Callback* callback);
    void unRegisterCallback(Callback* callback);
    void notifyCallbacks(ECS& ecs, EntityId entityA, EntityId entityB);
private:
    std::unordered_set<Callback*> mCallbacks;
};


#endif