#ifndef GAME_INCLUDE
#define GAME_INCLUDE

#define RAD_TO_DEGS 57.2957795
#define PI 3.1415927
#define TWO_PI 6.2831853

#include <vector>
#include <map>
#include <mutex>
#include <queue>
#include <iostream>
#include <memory>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_image.h"
#include "../SDL2/SDL_ttf.h"

#include "Util/Vector2.hpp"

typedef void (*func_ptr)(void); // for passing function ptrs to ui buttons

class Object;
class UIElement;
class Player;
class UIManager;
class ObjectManager;
class ContentManager;
class KeyboardHandler;
class MouseHandler;
class Registry;
class ComponentHandler;
class TransformComponent;
class AnimationManager;
class GridManager;

class Game
{
private:
    int _targetUPS = 60;  // physics updates
    int _targetFPS = 120; // aim for this many fps

    static Game *_pInstance;

    int _fps = 0, _ups = 0; // frames per sec, updates per sec

    std::mutex _mutex;

public:
    const int SPRITE_PPM = 16; // texture pixels per meter (determines scaling of sprite on screen by dimensions)

    ContentManager *pContentManager;
    KeyboardHandler *pKeyboardHandler;
    MouseHandler *pMouseHandler;
    UIManager *pUIManager;
    ObjectManager *pObjectManager;
    Registry *pRegistry;
    ComponentHandler *pComponentHandler;
    AnimationManager *pAnimationManager;
    GridManager *pGridManager;

    std::shared_ptr<Player> pPlayer;

    int winWidth = 1280, winHeight = 960; // window dimensions, 1250x835 by default in windowed mode
    int screenWidth, screenHeight;
    int ppm = 48; // screen pixels per meter, 48 by default
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

    Vector2 gridDims;

    Vector2 cameraPos;
    bool running = true;
    bool paused = false;

    int score = 0;

    static Game *getInstance(); // get singleton instance
    Game();
    ~Game();
    int windowInit();           // init window
    int gameInit();             // init game and manager class instances
    static void startGame();         // start game
    void start();
    void runPhysics();    // runs physics thread
    void pollEvents();    // polls input events for mouse and keyboard
    void frameUpdate();   // perform render cycle
    void physicsUpdate(); // perform physics cycle
    void draw();          // draw everything to screen

    Vector2Int worldToPixel(const Vector2 &pos);   // converts a world position to a pixel position
    Vector2 pixelToWorld(const Vector2Int &pxPos); // converts a pixel position to a world position

    bool isTransformOnScreen(TransformComponent &transform); // returns true if transform is on screen, false otherwise

    static void reset();
    static void togglePause();
    static void quit();

    inline int getFPS() { return _fps; }
    inline int getUPS() { return _ups; }
};

#endif