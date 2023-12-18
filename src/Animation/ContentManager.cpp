#include "../../include/game/Animation/ContentManager.hpp"
#include "../../include/game/Game.hpp"

#include <filesystem>

ContentManager::ContentManager() : _game(*Game::getInstance())
{
    // _game = Game::getInstance();
}

ContentManager::~ContentManager()
{
    destroyContent();
}

void ContentManager::loadContent()
{
    loadTextures();
    loadFonts();
}

void ContentManager::loadTextures()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    _textureFromName.insert({"", nullptr});

    SDL_Surface *pSurface;
    SDL_Texture *pTexture;

    for(const auto& dirEntry : recursive_directory_iterator(TEXTURE_PATH))
    {
        pSurface = IMG_Load((const char *)dirEntry.path().string().c_str());
        if(pSurface == NULL)
        {
            std::cerr << "ContentManager loadTextures(): Failed to load image to surface: " << IMG_GetError() << std::endl;
            return;
        }

        pTexture = SDL_CreateTextureFromSurface(_game.pRenderer, pSurface);
        if(pTexture == NULL)
        {
            std::cerr << "ContentManager loadTextures(): Failed to load texture from surface: " << IMG_GetError() << std::endl;
            return;
        }

        std::string fileName = dirEntry.path().filename().string();

        _textureFromName.insert({fileName.substr(0, fileName.length() - 4), pTexture});

        SDL_FreeSurface(pSurface);
    }

    _textureFromType[EntityType::Player] = _textureFromName["player-sheet"];
    _textureFromType[EntityType::Border] = _textureFromName["blackblock"];
    _textureFromType[EntityType::Block] = _textureFromName["block"];
    _textureFromType[EntityType::NotSet] = _textureFromName["blueblock"];

    _spriteSheetFromType[EntityType::Player] = _textureFromName["player-sheet"];
}

void ContentManager::loadFonts()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::string currPath;
    TTF_Font *font;

    for(const auto& dirEntry : recursive_directory_iterator(FONT_PATH))
    {
        currPath = dirEntry.path().filename().string();

        font = TTF_OpenFont((const char *)dirEntry.path().string().c_str(), 51);
        if(font == NULL)
        {
            std::cerr << "ContentManager loadFonts(): Failed to open font: " << TTF_GetError() << std::endl;
            return;
        }

        _fonts.insert({currPath.substr(0, currPath.length() - 4), font});
    }
}

void ContentManager::destroyContent()
{
    destroyTextures();
    closeFonts();
}

void ContentManager::destroyTextures()
{
    std::map<std::string, SDL_Texture *>::iterator it;
    for(it = _textureFromName.begin(); it != _textureFromName.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
}

void ContentManager::closeFonts()
{
    std::map<std::string, TTF_Font *>::iterator it;
    for(it = _fonts.begin(); it != _fonts.end(); it++)
    {
        TTF_CloseFont(it->second);
    }
}

SDL_Texture *ContentManager::getTexture(std::string textureName)
{
    return _textureFromName[textureName];
}

SDL_Texture *ContentManager::getTextureFromType(EntityType type)
{
    return _textureFromType[type];
}

SDL_Texture *ContentManager::getSpriteSheetFromType(EntityType type)
{
    return _spriteSheetFromType[type];
}

TTF_Font* ContentManager::getFont(std::string fontName)
{
    return _fonts[fontName];
}