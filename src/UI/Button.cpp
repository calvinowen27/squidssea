#include "../../include/game/UI/Button.hpp"
#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Game.hpp"

Button::Button() : UIElement()
{
}

bool Button::init(std::string textureName, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)(), bool isEnabled)
{
    UIElement::init(textureName, relativePos, PositionMode::ScreenRelative, relativeDims, ScaleMode::WidthRelative, isEnabled);

    _actionFunc = actionFunc;

    return true;
}

void Button::update()
{
    UIElement::update();

    if(mouseHandler.mouseWithinRect(drawRect) && mouseHandler.isButtonJustPressed(MOUSE_LEFT))
    {
        _actionFunc();
    }
}