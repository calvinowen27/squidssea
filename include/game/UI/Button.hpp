#ifndef BUTTON_INCLUDE
#define BUTTON_INCLUDE

#include "UIElement.hpp"

class Button : public UIElement
{
private:
    void (*_actionFunc)() = nullptr;

public:
    Button();
    bool init(std::string textureName, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)(), bool isEnabled = true);
    void update() override;
};

#endif