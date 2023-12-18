#include "../../include/game/Entity/LiveEntity.hpp"

LiveEntity::LiveEntity() : Entity()
{
}

bool LiveEntity::init(EntityType entityType, Vector2 pos, Vector2 dims)
{
    Entity::init(entityType, pos, dims);

    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
    pHitbox = registry.newComponent<HitboxComponent>();

    pRenderer->setCollider(pCollider);

    pRigidbody->init(entityType, pTransform, pCollider);

    pCollider->init(entityType, pTransform, pRigidbody);
    pCollider->setEntity(this);

    pHitbox->init(entityType, pTransform, pRigidbody);
    pHitbox->setEntity(this);

    pRenderer->setHitbox(pHitbox);

    // pHealth = registry.newComponent<HealthComponent>();
    // pHealth->init(10, pTransform, Vector2(0, pTransform->dims.y + 0.5));
    // pHealth->setEntity(this);

    return true;
}

void LiveEntity::update(float time)
{
    Entity::update(time);
}

void LiveEntity::kill()
{
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
    registry.killComponent(pHitbox);
    // registry.killComponent(pHealth);

    Entity::kill();
}