#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Input/KeyboardHandler.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/UI/UIManager.hpp"

std::map<SDL_Scancode, bool> inputState;

Player::Player() : LiveEntity()
{
    _pKeyboardHandler = game.pKeyboardHandler;
}

bool Player::init(Vector2 pos)
{
    LiveEntity::init(EntityType::Player, pos, Vector2(1, 1));

    pAnimator = registry.newComponent<AnimatorComponent>();
    pAnimator->init(pRenderer);
    pAnimator->setAnimation(EntityType::Player, "idle_down");

    _pCurrInteractable = nullptr;

    pTransform->setEntity(this);

    _moveDir = Vector2::zero;

    return true;
}

void Player::update(float time)
{
    if (!alive)
        return;

    LiveEntity::update(time);

    if(_pKeyboardHandler->isPressed(InputKey::Up))
    {
        setMoveDir(Vector2(0, 1));
    }

    if(_pKeyboardHandler->isPressed(InputKey::Down))
    {
        setMoveDir(Vector2(0, -1));
    }

    if(_pKeyboardHandler->isPressed(InputKey::Right))
    {
        setMoveDir(Vector2(1, 0));
    }

    if(_pKeyboardHandler->isPressed(InputKey::Left))
    {
        setMoveDir(Vector2(-1, 0));
    }
}

void Player::kill()
{
    LiveEntity::kill();

    registry.killComponent(pAnimator);

    _pCurrInteractable = nullptr;
}

void Player::onCollisionEnter(Entity *pOther)
{
    LiveEntity::onCollisionEnter(pOther);

    if (pOther)
    {
        EntityType otherType = pOther->getType();

        if (otherType == EntityType::Block || otherType == EntityType::Border)
        {
            _canChangeDir = true;
            _moveDir = Vector2::zero;
            pRigidbody->velocity = Vector2::zero;
            pTransform->pos = (Vector2)((Vector2Int)pTransform->pos);
        }
    }
}

void Player::onCollisionExit(Entity *pOther)
{
    LiveEntity::onCollisionExit(pOther);
}

bool Player::setMoveDir(Vector2 moveDir)
{
    if (_canChangeDir && game.pUIManager->getPieceUI()->allPiecesUsed())
    {
        _moveDir = moveDir;
        _canChangeDir = false;
        pRigidbody->velocity = _moveDir * _moveSpeed;

        if (moveDir.x > 0)
            pTransform->rotDeg = 90;
        else if (moveDir.x < 0)
            pTransform->rotDeg = 270;

        if (moveDir.y > 0)
            pTransform->rotDeg = 0;
        else if (moveDir.y < 0)
            pTransform->rotDeg = 180;

        return true;
    }

    return false;
}