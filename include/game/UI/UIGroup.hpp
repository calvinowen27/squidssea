#ifndef UI_GROUP_INCLUDE
#define UI_GROUP_INCLUDE

#include "UIElement.hpp"
#include "Button.hpp"
#include "TextElement.hpp"
#include "TextButton.hpp"

#include "../../SDL2/SDL.h"

#include <vector>
#include <memory>
#include <queue>

class Game;
class SpellManager;
class UIPiece;

class UIGroup
{
protected:
    Game &game;

    std::vector<std::shared_ptr<UIElement>> uiElements;
    bool isEnabled = true;

public:
    UIGroup();
    virtual void init();
    virtual void update();
    virtual void draw(SDL_Renderer *pRenderer);
    virtual void setEnabled(bool val);
    virtual bool toggleEnabled();
    inline bool getIsEnabled() { return isEnabled; }

    template <typename T>
    std::shared_ptr<T> newUIElement()
    {
        if (!std::is_base_of<UIElement, T>::value)
        {
            throw new std::invalid_argument("UIGroup::newUIElement<Type>() : Type must be derived from UIElement class.");
        }

        std::shared_ptr<T> element = std::make_shared<T>();
        uiElements.push_back(element);
        return element;
    }
};

class DebugUIGroup : public UIGroup
{
private:
    std::shared_ptr<TextElement> _pFPScounter;
    std::shared_ptr<TextElement> _pUPScounter;
    std::shared_ptr<TextElement> _pPosDisplay;
    std::shared_ptr<TextElement> _pVelDisplay;
    std::shared_ptr<TextElement> _pAccDisplay;
    std::shared_ptr<TextElement> _pPPMDisplay;

public:
    DebugUIGroup();
    void init() override;
    void update() override;
};

class PauseUIGroup : public UIGroup
{
private:
    std::shared_ptr<UIElement> _pPauseOverlay;
    std::shared_ptr<TextButton> _pResetButton;
    std::shared_ptr<TextButton> _pResumeButton;
    std::shared_ptr<TextButton> _pQuitButton;

public:
    PauseUIGroup();
    void init() override;
};

class PieceUIGroup : public UIGroup
{
private:
    std::shared_ptr<UIPiece> _pieces[3];

    std::shared_ptr<TextButton> _pPauseButton;

public:
    void init() override;
    void reset();
};

#endif