#include "../../include/game/Objects/Object.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"

#include "../../include/SDL2/SDL.h"
#include "../../include/SDL2/SDL_image.h"

Object::Object() : Entity()
{
}

bool Object::init(EntityType entityType, Vector2 pos, Vector2 dims)
{
    Entity::init(entityType, pos, dims);

    pRigidbody = registry.newComponent<RigidbodyComponent>();
    pCollider = registry.newComponent<ColliderComponent>();
    pHitbox = registry.newComponent<HitboxComponent>();

    pRenderer->setCollider(pCollider);

    pRigidbody->init(entityType, pTransform, pCollider);

    pCollider->init(entityType, pTransform, pRigidbody);
    pCollider->setEntity(this);

    if (pHitbox->init(entityType, pTransform, pRigidbody))
    {
        pHitbox->setEntity(this);
    }
    else
    {
        registry.killComponent(pHitbox);
        pHitbox = nullptr;
    }

    pRenderer->setHitbox(pHitbox);

    return true;
}

void Object::kill()
{
    Entity::kill();

    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);

    if(pHitbox)
        registry.killComponent(pHitbox);
}