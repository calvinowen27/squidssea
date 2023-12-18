#include "../../include/game/UI/UIPiece.hpp"
#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Objects/GridManager.hpp"
#include "../../include/game/Animation/ContentManager.hpp"

#include <time.h>

UIPiece::UIPiece() : UIElement()
{
}

bool UIPiece::init(Vector2 pos, Vector2 dims)
{
    _pieceType = pieceTypes[rand() % pieceTypes.size()];;

    UIElement::init(_pieceTextureFromType[_pieceType], pos, PositionMode::WorldAbsolute, dims, ScaleMode::WorldAbsolute, true, false);

    _startPos = pos;

    return true;
}

void UIPiece::update()
{
    if (!isEnabled)
        return;

    UIElement::update();

    if (uiManager.getPauseUI()->getIsEnabled())
        return;

    Vector2Int mousePxPos = mouseHandler.getMousePxPos();

    if ((mousePxPos.x >= pxPos.x && mousePxPos.x <= pxPos.x + pxDims.x &&
         mousePxPos.y >= pxPos.y && mousePxPos.y <= pxPos.y + pxDims.y) &&
        mouseHandler.isButtonJustPressed(SDL_BUTTON_LEFT) && !_isDragging)
    {
        _isDragging = true;
    }
    else if (_isDragging)
    {
        if (!mouseHandler.isButtonPressed(SDL_BUTTON_LEFT))
        {
            _isDragging = false;

            Vector2Int gridCellPos = (Vector2Int)mouseHandler.getMousePos();

            if (game.pGridManager->placePiece(gridCellPos, _pieceType))
            {
                disable();
                reset();
            }
        }
        else
        {            
            pxPos = mousePxPos - Vector2Int(game.ppm / 2, pxDims.y - game.ppm / 2);
        }
    }

    if(!_isDragging)
        pxPos = game.worldToPixel(_startPos);

    drawRect = SDL_Rect{pxPos.x, pxPos.y, pxDims.x, pxDims.y};
}

void UIPiece::reset()
{
    PieceType nextPieceType = pieceTypes[rand() % pieceTypes.size()];
    _pieceType = nextPieceType;
    pTexture = contentManager.getTexture(_pieceTextureFromType[_pieceType]);
}