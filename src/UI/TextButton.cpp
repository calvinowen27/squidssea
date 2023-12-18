#include "../../include/game/UI/TextButton.hpp"
#include "../../include/game/Input/MouseHandler.hpp"

TextButton::TextButton() : TextElement()
{
    _actionFunc = nullptr;
}

bool TextButton::init(std::string textureName, std::string buttonText, std::string fontName, SDL_Color textColor, Vector2 relativePos, Vector2 relativeDims, func_ptr actionFunc, bool isEnabled)
{
    TextElement::init(textureName, buttonText, fontName, textColor, relativePos, relativeDims, false, true, isEnabled);

    _actionFunc = actionFunc;

    return true;
}

void TextButton::update()
{
    TextElement::update();

    if(mouseHandler.mouseWithinRect(drawRect) && mouseHandler.isButtonJustPressed(MOUSE_LEFT))
    {
        _actionFunc();
    }
}