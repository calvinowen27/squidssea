#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Objects/GridManager.hpp"
#include "../../include/game/UI/UIPiece.hpp"

MouseHandler::MouseHandler() : _game(*Game::getInstance())
{
}

void MouseHandler::update()
{
    _prevButtonState = _currButtonState;
    _pxPrevMousePos = _pxMousePos;
    _currButtonState = SDL_GetMouseState(&_pxMousePos.x, &_pxMousePos.y);

    _prevMousePos = _mousePos;
    _mousePos = _game.pixelToWorld(_pxMousePos);
}

void MouseHandler::onMouseButtonDown(int button)
{
}

void MouseHandler::onMouseButtonUp(int button)
{
}

bool MouseHandler::isButtonPressed(int button)
{
    return _currButtonState & SDL_BUTTON(button);
}

bool MouseHandler::wasButtonPressed(int button)
{
    return _prevButtonState & SDL_BUTTON(button);
}

bool MouseHandler::isButtonJustPressed(int button)
{
    return isButtonPressed(button) && !wasButtonPressed(button);
}

bool MouseHandler::isButtonJustReleased(int button)
{
    return !isButtonPressed(button) && wasButtonPressed(button);
}

void MouseHandler::mouseWheel(int wheelY)
{
}

bool MouseHandler::mouseWithinRect(SDL_Rect rect)
{
    return !(_pxMousePos.x < rect.x || _pxMousePos.x > rect.x + rect.w || _pxMousePos.y < rect.y || _pxMousePos.y > rect.y + rect.h);
}