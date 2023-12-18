#include "../../include/game/UI/UIElement.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"

UIElement::UIElement() : game(*Game::getInstance()), contentManager(*game.pContentManager), uiManager(*game.pUIManager), mouseHandler(*game.pMouseHandler)
{
}

bool UIElement::init(std::string textureName, Vector2 pos, PositionMode positionMode, Vector2 dims, ScaleMode scaleMode, bool enabled, bool isStatic)
{
    this->isEnabled = enabled;
    this->isStatic = isStatic;
    pTexture = contentManager.getTexture(textureName);

    this->pos = pos;
    this->positionMode = positionMode;
    this->dims = dims;
    this->scaleMode = scaleMode;

    if (positionMode == PositionMode::ScreenRelative)
    {
        pxPos = (Vector2Int)(pos * Vector2(game.winWidth, game.winHeight));
    }
    else if (positionMode == PositionMode::WorldAbsolute)
    {
        pxPos = game.worldToPixel(pos);
    }

    if (scaleMode == ScaleMode::WidthRelative)
    {
        pxDims = (Vector2Int)(dims * Vector2(game.winWidth, game.winWidth));
    }
    else if (scaleMode == ScaleMode::WorldAbsolute)
    {
        pxDims = (Vector2Int)(dims * game.ppm);
    }

    drawRect = SDL_Rect{pxPos.x, pxPos.y, pxDims.x, pxDims.y};

    return true;
}

void UIElement::update()
{
    if (isStatic)
    {
        if (positionMode == PositionMode::ScreenRelative)
        {
            pxPos = (Vector2Int)(pos * Vector2(game.winWidth, game.winHeight));
        }
        else if (positionMode == PositionMode::WorldAbsolute)
        {
            pxPos = game.worldToPixel(pos);
        }
    }

    if (scaleMode == ScaleMode::WidthRelative)
    {
        pxDims = (Vector2Int)(dims * Vector2(game.winWidth, game.winWidth));
    }
    else if (scaleMode == ScaleMode::WorldAbsolute)
    {
        pxDims = (Vector2Int)(dims * game.ppm);
    }

    drawRect = SDL_Rect{pxPos.x, pxPos.y, pxDims.x, pxDims.y};
}

void UIElement::draw(SDL_Renderer *pRenderer)
{
    SDL_RenderCopy(pRenderer, pTexture, NULL, &drawRect);
}
