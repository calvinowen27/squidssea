#ifndef INTERACTABLE_INCLUDE
#define INTERACTABLE_INCLUDE

#include "Object.hpp"

class InteractableObject : public Object
{
public:
    bool init(EntityType entityType, Vector2 pos, Vector2 dims = Vector2(1, 1)) override;
    virtual bool interact();

    void onCollisionEnter(Entity *pOther) override;
    void onCollisionExit(Entity *pOther) override;
};

#endif