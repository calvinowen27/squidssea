#ifndef LIVE_ENTITY_INCLUDE
#define LIVE_ENTITY_INCLUDE

#include "Entity.hpp"
#include "../Util/Vector2.hpp"

class LiveEntity : public Entity
{
protected:
    std::shared_ptr<RigidbodyComponent> pRigidbody;
    std::shared_ptr<ColliderComponent> pCollider;
    std::shared_ptr<HitboxComponent> pHitbox;
    // std::shared_ptr<HealthComponent> pHealth;

public:
    LiveEntity();
    bool init(EntityType entityType, Vector2 pos, Vector2 dims = Vector2(1, 1)) override;
    void update(float time) override;
    void kill() override;

    inline Vector2 getVelocity() { return pRigidbody->velocity; }
    inline std::shared_ptr<ColliderComponent> getCollider() { return pCollider; }
    // inline bool damage(float damage) { return pHealth->damage(damage); }
};

#endif