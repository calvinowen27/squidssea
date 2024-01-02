#include "../../include/game/UI/UIGroup.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Entity/Player.hpp"

/* UI GROUP */
UIGroup::UIGroup() : game(*Game::getInstance())
{
}

void UIGroup::init()
{
}

void UIGroup::update()
{
    for (auto el : uiElements)
        el->update();
}

void UIGroup::draw(SDL_Renderer *pRenderer)
{
    for (auto el : uiElements)
    {
        if (el->getIsEnabled())
            el->draw(pRenderer);
    }
}

void UIGroup::setEnabled(bool val)
{
    isEnabled = val;

    for (auto el : uiElements)
        el->setEnabled(val);
}

bool UIGroup::toggleEnabled()
{
    setEnabled(!isEnabled);
    return isEnabled;
}

/* DEBUG UI GROUP */
DebugUIGroup::DebugUIGroup() : UIGroup()
{
}

void DebugUIGroup::init()
{
    _pFPScounter = newUIElement<TextElement>();
    _pFPScounter->init("ui_text_back", "FPS: ", "arial", WHITE, Vector2::zero, Vector2(0.1, 0.0175));

    _pUPScounter = newUIElement<TextElement>();
    _pUPScounter->init("ui_text_back", "UPS: ", "arial", WHITE, Vector2(0, 0.026), Vector2(0.1, 0.0175));

    _pPPMDisplay = newUIElement<TextElement>();
    _pPPMDisplay->init("ui_text_back", "PPM: ", "arial", WHITE, Vector2(0, 0.052), Vector2(0.1, 0.0175));

    _pPosDisplay = newUIElement<TextElement>();
    _pPosDisplay->init("ui_text_back", "Position: ", "arial", WHITE, Vector2(0.7, 0), Vector2(0.3, 0.0175));

    _pVelDisplay = newUIElement<TextElement>();
    _pVelDisplay->init("ui_text_back", "Velocity: ", "arial", WHITE, Vector2(0.7, 0.026), Vector2(0.3, 0.0175));

    UIGroup::init();

    setEnabled(true);
}

void DebugUIGroup::update()
{
    _pFPScounter->setText("FPS: " + std::to_string(game.getFPS()));
    _pUPScounter->setText("UPS: " + std::to_string(game.getUPS()));
    _pPPMDisplay->setText("PPM: " + std::to_string(game.ppm));
    if (game.pPlayer && game.pPlayer->isAlive())
    {
        _pPosDisplay->setText("Position: " + game.pPlayer->getPos().round(3).to_string());
        _pVelDisplay->setText("Velocity: " + game.pPlayer->getVelocity().round(3).to_string());
    }

    UIGroup::update();
}

/* PAUSE UI GROUP */
PauseUIGroup::PauseUIGroup() : UIGroup()
{
}

void PauseUIGroup::init()
{
    _pPauseOverlay = newUIElement<UIElement>();
    _pPauseOverlay->init("pause_overlay", Vector2::zero, PositionMode::ScreenRelative, Vector2(1, 1), ScaleMode::WidthRelative, false);

    _pResetButton = newUIElement<TextButton>();
    _pResetButton->init("ui_button", "Reset", "arial", WHITE, Vector2(0.4, 0.15), Vector2(0.25, 0.05), Game::reset);

    _pResumeButton = newUIElement<TextButton>();
    _pResumeButton->init("ui_button", "Resume", "arial", WHITE, Vector2(0.4, 0.25), Vector2(0.25, 0.05), Game::togglePause);

    _pQuitButton = newUIElement<TextButton>();
    _pQuitButton->init("ui_button", "Quit", "arial", WHITE, Vector2(0.4, 0.35), Vector2(0.25, 0.05), Game::quit);

    UIGroup::init();

    setEnabled(false);
}

/* PIECE UI GROUP */
void PieceUIGroup::init()
{
    UIGroup::init();

    srand(time(NULL));

    _pieces[0] = newUIElement<UIPiece>();
    _pieces[0]->init(Vector2(-1, -3), Vector2(3, 3));

    _pieces[1] = newUIElement<UIPiece>();
    _pieces[1]->init(Vector2(3, -3), Vector2(3, 3));

    _pieces[2] = newUIElement<UIPiece>();
    _pieces[2]->init(Vector2(7, -3), Vector2(3, 3));

    _pPauseButton = newUIElement<TextButton>();
    _pPauseButton->init("ui_button", "ii", "arial", WHITE, Vector2(0.0125, 0.85), Vector2(0.0375, 0.0375), Game::togglePause);

    _pScoreCounter = newUIElement<TextElement>();
    _pScoreCounter->init("", "Score: 0", "arial", WHITE, Vector2(0.45, 0.05), Vector2(0.1, 0.025), false, true);

    setEnabled(false);
}

void PieceUIGroup::update()
{
    UIGroup::update();

    _pScoreCounter->setText("Score: " + std::to_string(game.score));
}

void PieceUIGroup::reset()
{
    _pieces[0]->reset();
    _pieces[1]->reset();
    _pieces[2]->reset();
}

bool PieceUIGroup::allPiecesUsed()
{
    return !_pieces[0]->getIsEnabled() && !_pieces[1]->getIsEnabled() && !_pieces[2]->getIsEnabled();
}


/* START MENU UI GROUP */
void StartMenuUIGroup::init()
{
    UIGroup::init();

    _pStartButton = newUIElement<TextButton>();
    _pStartButton->init("ui_button", "start", "arial", WHITE, Vector2(0.4, 0.25), Vector2(0.25, 0.05), Game::startGame);

    setEnabled(true);
}