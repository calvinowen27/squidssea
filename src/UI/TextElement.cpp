#include "../../include/game/UI/TextElement.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"

TextElement::TextElement() : UIElement()
{
}

TextElement::~TextElement()
{
    SDL_DestroyTexture(_texture);
    SDL_FreeSurface(_surface);
}

bool TextElement::init(std::string textureName, std::string text, std::string fontName, SDL_Color color, Vector2 relativePos, Vector2 relativeDims, bool displayOutline, bool centerText, bool isEnabled)
{
    UIElement::init(textureName, relativePos, PositionMode::ScreenRelative, relativeDims, ScaleMode::WidthRelative, isEnabled);

    _pxTextWidth = pxDims.x;
    _color = color;
    _displayOutline = displayOutline;
    _centerText = centerText;
    setFont(fontName);
    setText(text);

    return true;
}

void TextElement::draw(SDL_Renderer *pRenderer)
{
    UIElement::draw(pRenderer);

    SDL_RenderCopy(pRenderer, _texture, NULL, &_textRect);

    SDL_SetRenderDrawColor(pRenderer, 50, 50, 50, 142);

    if(_displayOutline) SDL_RenderDrawRect(pRenderer, &drawRect);
}

void TextElement::update()
{
    UIElement::update();

    _pxTextWidth = ((float)_textDims.x/(float)_textDims.y)*dims.y*0.85*game.winWidth;
    _textRect = drawRect;
    if(_pxTextWidth > drawRect.w) 
    {
        _pxTextWidth = drawRect.w*0.65;
        _textRect.h = ((float)_textDims.y/(float)_textDims.x)*dims.x*game.winHeight;
    }

    _textRect.w = _pxTextWidth;

    if(_centerText)
    {
        _textRect.x = drawRect.x + (drawRect.w - _textRect.w) / 2;
        _textRect.y = drawRect.y + (drawRect.h - _textRect.h) / 2;
    }
}

void TextElement::setText(std::string newText)
{
    // measure dims
    TTF_SizeUTF8(_pFont, newText.c_str(), &_textDims.x, &_textDims.y);

    _pxTextWidth = ((float)_textDims.x/(float)_textDims.y)*dims.y*0.9*game.winWidth;
    _textRect = drawRect;
    if(_pxTextWidth > drawRect.w) 
    {
        _pxTextWidth = drawRect.w;
        _textRect.h = ((float)_textDims.x/(float)_textDims.y)*dims.x*game.winHeight;
    }

    _textRect.w = _pxTextWidth;

    _text = newText;

    if(_texture != nullptr) SDL_DestroyTexture(_texture);

    _surface = TTF_RenderText_Solid(_pFont, _text.c_str(), _color);
    if(_surface == NULL)
    {
        std::cerr << "TextElement: setText(): Failed to create surface: " << TTF_GetError() << std::endl;
    }
    _texture = SDL_CreateTextureFromSurface(game.pRenderer, _surface);
    if(_texture == NULL)
    {
        std::cout << "TextElement: setText(): Failed to create texture: " << IMG_GetError() << std::endl;
    }

    SDL_FreeSurface(_surface);
}

std::string TextElement::getText()
{
    return _text;
}

void TextElement::setFont(std::string fontName)
{
    _pFont = contentManager.getFont(fontName);
}