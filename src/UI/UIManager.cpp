#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Entity/Player.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/TextButton.hpp"
#include "../../include/game/UI/Button.hpp"
#include "../../include/game/Input/KeyboardHandler.hpp"

#include <cmath>

UIManager *UIManager::_pInstance;

UIManager::UIManager() : _game(*Game::getInstance())
{
}

UIManager *UIManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new UIManager();
    }

    return _pInstance;
}

void UIManager::init()
{
    _pPieceUI = newUIGroup<PieceUIGroup>();
    _pPieceUI->init();

    _pDebugUI = newUIGroup<DebugUIGroup>();
    _pDebugUI->init();

    _pPauseUI = newUIGroup<PauseUIGroup>();
    _pPauseUI->init();
}

void UIManager::update()
{
    // el.update()
    for (auto group : _uiGroups)
    {
        if (group->getIsEnabled())
            group->update();
    }
}

void UIManager::draw(SDL_Renderer *pRenderer)
{
    // el.draw();
    for (auto group : _uiGroups)
    {
        if (group->getIsEnabled())
            group->draw(pRenderer);
    }
}