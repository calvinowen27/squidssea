#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

// #include "Game.h"
#include "LiveEntity.hpp"
#include <map>

class KeyboardHandler;
class InteractableObject;

class Player : public LiveEntity
{
private:
    KeyboardHandler *_pKeyboardHandler;

    float _moveSpeed = 5; // meters/sec

    InteractableObject *_pCurrInteractable;

    bool _canChangeDir = true;
    Vector2 _moveDir;

public:
    std::shared_ptr<AnimatorComponent> pAnimator;

    Player();
    bool init(Vector2 pos);
    void update(float time) override;
    void kill() override;

    void onCollisionEnter(Entity *pEntity) override;
    void onCollisionExit(Entity *pEntity) override;

    inline void setPos(Vector2 newPos) { pTransform->pos = newPos; }
    inline void setVelocity(Vector2 newVelocity) { pRigidbody->velocity = newVelocity; }
    inline void setCurrInteractable(InteractableObject *pInteractable) { _pCurrInteractable = pInteractable; }
    inline InteractableObject *getCurrInteractable() { return _pCurrInteractable; }

    inline void enableMovement() { _canChangeDir = true; }

    bool setMoveDir(Vector2 moveDir);
};

#endif