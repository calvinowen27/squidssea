#ifndef TEXT_BUTTON_INCLUDE
#define TEXT_BUTTON_INCLUDE

#include "TextElement.hpp"
#include "../Game.hpp"

class TextButton : public TextElement
{
private:
    func_ptr _actionFunc;

public:
    TextButton();
    bool init(std::string textureName, std::string buttonText, std::string fontName, SDL_Color textColor, Vector2 relativePos, Vector2 relativeDims, func_ptr actionFunc, bool isEnabled = true);
    void update() override;
};

#endif