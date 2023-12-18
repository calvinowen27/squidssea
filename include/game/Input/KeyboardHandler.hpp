#ifndef KEYBOARD_INCLUDE
#define KEYBOARD_INCLUDE

#include "../../SDL2/SDL.h"
#include <queue>
#include <map>

enum class InputKey {
    Right,
    Left,
    Up,
    Down,
    Pause,
    ToggleDebugUI,
    ToggleShowColliders,
    Interact
};

class Game;

class KeyboardHandler
{
    private:
        Game &_game;

        const Uint8 *_pKeyboardState;
        Uint8 _prevKeyboardState;
        std::queue<SDL_Event> _inputEvents;
        std::map<InputKey, SDL_Scancode> _keybinds;
        std::map<SDL_Scancode, bool> _keyDown;

    public:
        KeyboardHandler();
        ~KeyboardHandler();
        void processInputs();
        void onKeyUp(SDL_Scancode scancode);
        void onKeyDown(SDL_Scancode scancode);
        bool isPressed(InputKey key);
        void pushEvent(SDL_Event event);
};

#endif