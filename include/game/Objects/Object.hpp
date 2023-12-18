#ifndef OBJECT_INCLUDE
#define OBJECT_INCLUDE

#include <memory>

#include "../../SDL2/SDL.h"

#include "../Entity/Entity.hpp"

class Game;
class ObjectManager;
class ContentManager;

class Object : public Entity
{
protected:
    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<ColliderComponent> pCollider;
    std::shared_ptr<HitboxComponent> pHitbox;

public:
    Object();
    bool init(EntityType entityType, Vector2 pos, Vector2 dims = Vector2(1, 1)) override;
    void kill() override;

    inline Vector2 getVelocity() { return pRigidbody->velocity; }
    inline std::shared_ptr<ColliderComponent> getCollider() { return pCollider; }
};

#endif