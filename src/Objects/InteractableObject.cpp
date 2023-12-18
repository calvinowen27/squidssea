#include "../../include/game/Objects/InteractableObject.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Entity/Player.hpp"

bool InteractableObject::init(EntityType entityType, Vector2 pos, Vector2 dims)
{
    Object::init(entityType, pos, dims);

    return true;
}

bool InteractableObject::interact()
{
    return true;
}

void InteractableObject::onCollisionEnter(Entity *pOther)
{
    if (pOther && pOther->getType() == EntityType::Player)
        game.pPlayer->setCurrInteractable(this);
}

void InteractableObject::onCollisionExit(Entity *pOther)
{
    if (pOther && pOther->getType() == EntityType::Player && game.pPlayer->getCurrInteractable() == this)
        game.pPlayer->setCurrInteractable(nullptr);
}