#ifndef UIELEMENT_INCLUDE
#define UIELEMENT_INCLUDE

#include <string>

#include "../../SDL2/SDL.h"
#include "../../SDL2/SDL_image.h"
#include "../../SDL2/SDL_ttf.h"

#include "../Util/Vector2.hpp"

class Game;
class UIManager;
class ContentManager;
class MouseHandler;

enum class ScaleMode
{
    WidthRelative,
    WorldAbsolute
};

enum class PositionMode
{
    ScreenRelative,
    WorldAbsolute
};

class UIElement
{
    protected:
        Game &game;
        UIManager &uiManager;
        ContentManager &contentManager;
        MouseHandler &mouseHandler;

        Vector2 pos;
        Vector2 dims;
        Vector2Int pxPos;
        Vector2Int pxDims;

        SDL_Texture *pTexture;
        SDL_Rect drawRect;

        bool isEnabled = true;

        ScaleMode scaleMode;
        PositionMode positionMode;

        bool isStatic = true;

    public:
        UIElement();
        bool init(std::string textureName, Vector2 pos, PositionMode positionMode, Vector2 dims, ScaleMode scaleMode, bool enabled = true, bool isStatic = true);
        virtual void draw(SDL_Renderer* pRenderer);
        virtual void update();

        inline void enable() { isEnabled = true; }
        inline void disable() { isEnabled = false; }
        inline void setEnabled(bool val) { isEnabled = val; }
        inline bool getIsEnabled() { return isEnabled; }
};

#endif