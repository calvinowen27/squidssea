#ifndef COMPONENT_HANDLER_INCLUDE
#define COMPONENT_HANDLER_INCLUDE

#include "../../SDL2/SDL.h"

#include <vector>
#include <memory>
#include <mutex>

class Game;
class Registry;
class RendererComponent;
class Vector2;
class ColliderComponent;

class ComponentHandler
{
private:
    Game &_game;
    Registry &_registry;

    bool _showColliders = false;

public:
    ComponentHandler();
    void draw(SDL_Renderer *pRenderer);
    void update(float time);

    void updateColliders(float time);
    Vector2 handleCollision(std::shared_ptr<ColliderComponent> pCol1, std::shared_ptr<ColliderComponent> pCol2, Vector2 nextPos, Vector2 dPos);

    void updateRigidbodies(float time);
    void updateHealthComponents(float time);
    void updateTransforms(float time);
    void updateHitboxes(float time);
    void updateAnimators(float time);

    inline bool isShowingColliders() { return _showColliders; }
    inline void showColliders() { _showColliders = true; }
    inline void hideColliders() { _showColliders = false; }
    inline bool toggleShowColliders() { return _showColliders = !_showColliders; }
};

bool rendererComparator(std::shared_ptr<RendererComponent> a, std::shared_ptr<RendererComponent> b);

#endif