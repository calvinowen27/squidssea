#define SDL_MAIN_HANDLED

#include "../include/game/Game.hpp"
#include "../include/game/UI/TextElement.hpp"
#include "../include/game/Objects/Object.hpp"
#include "../include/game/Entity/Player.hpp"
#include "../include/game/UI/UIElement.hpp"
#include "../include/game/UI/UIManager.hpp"
#include "../include/game/Input/MouseHandler.hpp"
#include "../include/game/Input/KeyboardHandler.hpp"
#include "../include/game/Animation/ContentManager.hpp"
#include "../include/game/Objects/ObjectManager.hpp"
#include "../include/game/Entity/Registry.hpp"
#include "../include/game/Entity/Components.hpp"
#include "../include/game/Entity/ComponentHandler.hpp"
#include "../include/game/Animation/AnimationManager.hpp"
#include "../include/game/Objects/Block.hpp"
#include "../include/game/Objects/GridManager.hpp"

#include <thread>
#include <chrono>

Game *Game::_pInstance;

int main()
{
    Game *pGame = Game::getInstance();
    if (pGame->init())
    {
        return EXIT_FAILURE;
    }

    pGame->start();

    return 0;
}

Game::Game()
{
}

Game::~Game()
{
    delete pKeyboardHandler;
    delete pContentManager;
    delete pUIManager;
    delete pMouseHandler;
    delete pObjectManager;
    delete pRegistry;
    delete pComponentHandler;
    delete pAnimationManager;
    delete pGridManager;
}

Game *Game::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new Game();
    }

    return _pInstance;
}

int Game::init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cerr << "Error with SDL_image initialization: " << IMG_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    TTF_Init();

    float ddpi, hdpi, vdpi;
    SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi);
    winWidth *= hdpi / 96.f;
    winHeight *= vdpi / 96.f;
    ppm *= ddpi / 96.f;

    Uint64 windowFlags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

    pWindow = SDL_CreateWindow("magic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               winWidth, winHeight, windowFlags);

    // SDL_DisplayMode DM;
    // SDL_GetCurrentDisplayMode(0, &DM);
    // winWidth = DM.w;
    // winHeight = DM.h;

    // SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (pWindow == NULL)
    {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
    if (pRenderer == NULL)
    {
        std::cerr << "Failed to create renderer " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    gridDims = Vector2(9, 9);
    cameraPos = (gridDims / 2) - Vector2(0.5, 2);

    pRegistry = new Registry();
    pComponentHandler = new ComponentHandler();
    pKeyboardHandler = new KeyboardHandler();
    pMouseHandler = new MouseHandler();
    pContentManager = new ContentManager();
    pUIManager = UIManager::getInstance();
    pObjectManager = ObjectManager::getInstance();
    pAnimationManager = new AnimationManager();
    pGridManager = new GridManager();

    pContentManager->loadContent();
    pUIManager->init();
    pObjectManager->init();
    pAnimationManager->init();
    pGridManager->init(Vector2Int(9, 9));

    pPlayer = pObjectManager->newEntity<Player>();
    pPlayer->init(Vector2(0, 3));

    // auto block = pObjectManager->newEntity<Block>();
    // block->init(Vector2::zero);
    // obj->init(EntityType::Border, Vector2(1, 1));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(4, 6));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(3, 7));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(2, 5));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(3, 2));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(8, 0));

    // obj = pObjectManager->newEntity<Object>();
    // obj->init(EntityType::Border, Vector2(5, 4));

    auto obj = pObjectManager->newEntity<Object>();
    obj->init(EntityType::Border, Vector2(-1, -1), Vector2(1, gridDims.y + 2));

    obj = pObjectManager->newEntity<Object>();
    obj->init(EntityType::Border, Vector2(gridDims.x, -1), Vector2(1, gridDims.y + 2));

    obj = pObjectManager->newEntity<Object>();
    obj->init(EntityType::Border, Vector2(0, -1), Vector2(gridDims.x, 1));

    obj = pObjectManager->newEntity<Object>();
    obj->init(EntityType::Border, Vector2(0, gridDims.y), Vector2(gridDims.x, 1));

    return 0;
}

void Game::start()
{
    std::thread updateThread([this]
                             { runPhysics(); });

    while (_pInstance->running)
    {
        frameUpdate();
    }

    updateThread.join();

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::runPhysics()
{
    while (_pInstance->running)
    {
        physicsUpdate();
    }
}

void Game::pollEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }

        /* ------------- KEYBOARD INPUTS ------------------------ */
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            pKeyboardHandler->pushEvent(event);
        /* ------------------------------------------------------ */

        /* ------------ MOUSE INPUTS ---------------------------- */
        if (event.type == SDL_MOUSEWHEEL)
            pMouseHandler->mouseWheel(event.wheel.y);

        if (event.type == SDL_MOUSEBUTTONDOWN)
            pMouseHandler->onMouseButtonDown(event.button.button);

        if (event.type == SDL_MOUSEBUTTONUP)
            pMouseHandler->onMouseButtonUp(event.button.button);
        /* ------------------------------------------------------- */

        /* RESIZE WINDOW */
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            SDL_Window *win = SDL_GetWindowFromID(event.window.windowID);
            if (win == pWindow)
            {
                int oldWidth = winWidth;
                SDL_GetWindowSize(win, &winWidth, &winHeight);

                ppm *= (float)winWidth / (float)oldWidth;
            }
        }
    }
}

void Game::frameUpdate()
{
    using namespace std::chrono;
    float frameTime = 1 / (float)_targetFPS;
    nanoseconds timeDiff((int)(frameTime * 1000000000));

    auto startTime = high_resolution_clock::now();

    pollEvents();

    pMouseHandler->update();

    pUIManager->update();

    // set camera position to follow player
    // if (pPlayer && pPlayer->isAlive())
    //     cameraPos = pPlayer->getPos() + Vector2(0, 0.5);

    draw();

    auto execTime = duration_cast<nanoseconds>(high_resolution_clock::now() - startTime);

    // sleep for any extra time we have in the update
    auto sleepTime = timeDiff - execTime;

    std::lock_guard<std::mutex> guard(_mutex);

    _fps = 1000000000 / (sleepTime.count() > 0 ? timeDiff.count() : execTime.count());

    if (sleepTime.count() > 0)
        std::this_thread::sleep_for(sleepTime);
}

void Game::physicsUpdate()
{
    using namespace std::chrono;
    float updateTime = 1 / (float)_targetUPS; // seconds

    nanoseconds timeDiff((int)(updateTime * 1000000000)); // convert updateTime to nanoseconds

    auto startTime = high_resolution_clock::now();

    pKeyboardHandler->processInputs();

    if (paused) // don't update any of the following 
        return;

    pObjectManager->update(updateTime); // call update function on all objects

    pComponentHandler->update(updateTime); // call update function on all entity components

    auto execTime = duration_cast<nanoseconds>(high_resolution_clock::now() - startTime);

    // sleep for any extra time we have in the update
    auto sleepTime = timeDiff - execTime;

    std::lock_guard<std::mutex> guard(_mutex);

    _ups = 1000000000 / (sleepTime.count() > 0 ? timeDiff.count() : execTime.count());

    if (sleepTime.count() > 0)
        std::this_thread::sleep_for(sleepTime);
}

void Game::draw()
{
    SDL_RenderClear(pRenderer);

    pComponentHandler->draw(pRenderer); // calls draw function on all renderer components with render ordering

    pUIManager->draw(pRenderer); // draw all ui elements

    SDL_SetRenderDrawColor(pRenderer, 9, 106, 189, 255); // green background color (temporary)

    SDL_RenderPresent(pRenderer);
}

Vector2Int Game::worldToPixel(const Vector2 &pos)
{
    Vector2 pxPos = pos;
    // pxPos.y /= 2;                                                                // half y value to account for perspective
    pxPos -= cameraPos; // / Vector2(1, 2);                                          // half cameraPos.y to account for perspective, make pxPos relative to cameraPos
    pxPos *= ppm;                                                                // convert from meters to pixels
    pxPos.y *= -1;                                                               // invert y relative to window (pixel y positioning is from top not bottom)
    return (Vector2Int)(pxPos + Vector2(0.9, 0.9)) + Vector2Int((winWidth / 2) + 0.9, (winHeight / 2) + 0.9); // add half of window dimensions to center on screen
}

Vector2 Game::pixelToWorld(const Vector2Int &pxPos)
{
    Vector2 pos = (Vector2)pxPos;
    pos.x -= winWidth / 2;           // subtract half of window width to convert x to pixel positioning
    pos.y = (winHeight / 2) - pos.y; // convert y to pixel positioning with inversion in 1 step
    pos /= ppm;                      // convert from pixels to meters
    pos += cameraPos;                // add cameraPos for relative camera positioning
    return pos;
}

bool Game::isTransformOnScreen(TransformComponent &transform)
{
    return transform.pxPos.x <= winWidth && transform.pxPos.x + transform.pxDims.x >= 0 &&
           transform.pxPos.y <= winHeight && transform.pxPos.y + transform.pxDims.y >= 0;
}

// static
void Game::reset()
{
    togglePause();

    auto game = Game::getInstance();

    game->pGridManager->clearGrid();

    game->pPlayer->setPos(Vector2(0, 3));
    game->pPlayer->setMoveDir(Vector2(0, 1));
    game->pPlayer->setVelocity(Vector2::zero);
    game->pPlayer->enableMovement();

    game->pUIManager->getPieceUI()->reset();

    game->score = 0;
}

// static
void Game::togglePause()
{
    auto game = Game::getInstance();
    game->paused = !game->paused;
    game->pUIManager->getPauseUI()->toggleEnabled();
}

// static
void Game::quit()
{
    Game::getInstance()->running = false;
}