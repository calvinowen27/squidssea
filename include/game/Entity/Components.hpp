#ifndef COMPONENTS_INCLUDE
#define COMPONENTS_INCLUDE

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>

#include "../../SDL2/SDL.h"

#include "../Util/Vector2.hpp"

class Game;
class ContentManager;
class TransformComponent;
class RigidbodyComponent;
class Registry;
class Object;
class Entity;
class ObjectManager;
class Animation;
class AnimationManager;
class ColliderComponent;
class HitboxComponent;

enum class EntityType;

class Component
{
protected:
    Game &game;
    Registry &registry;
    ObjectManager &objectManager;

    Entity *pEntity;

    bool isEnabled;

public:
    Component();
    bool init();
    virtual void kill();
    void setEntity(Entity *pEntity);
    void enable() { isEnabled = true; }
    void disable() { isEnabled = false; }
    bool getIsEnabled() { return isEnabled; }
};

class RendererComponent : public Component
{
public:
    ContentManager &contentManager;

    SDL_Texture *pTexture;
    SDL_Rect spriteRect;
    SDL_Rect sourceRect;

    Vector2Int spriteDims;

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<ColliderComponent> pCollider;
    std::shared_ptr<HitboxComponent> pHitbox;

    bool isFlipped = false; // over y axis

    int renderOrder = 0;

    RendererComponent();
    bool init(std::string textureName, std::shared_ptr<TransformComponent> pTransform, int renderOrder = 0);                          // returns true if successful
    bool init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, int renderOrder = 0, bool startEnabled = false); // returns true if successful
    void update(float time);
    void draw(SDL_Renderer *pRenderer);
    void kill() override;
    bool setTexture(std::string textureName); // returns true if successful
    void refreshDimensions();
    inline void setCollider(std::shared_ptr<ColliderComponent> pCollider) { this->pCollider = pCollider; }
    inline void setHitbox(std::shared_ptr<HitboxComponent> pHitbox) { this->pHitbox = pHitbox; }
};

class TransformComponent : public Component
{
public:
    Vector2 pos;       // meters
    Vector2Int pxPos;  // pixels
    Vector2 dims;      // meters
    Vector2Int pxDims; // pixels

    float rotDeg; // angle in degrees
    float rotRad; // angle in radians

    bool constrain = true;

    TransformComponent();
    bool init(Vector2 pos, Vector2 dims = Vector2(1, 1), bool constrain = true);                        // returns true if successful
    bool init(EntityType entityType, Vector2 pos, Vector2 dims = Vector2(1, 1)); // returns true if successful

    void update(float time);

    void setPxDims(Vector2Int newPxDims); // set new px dims

    void updatePxPos();
    void updatePxDims();
};

class ColliderComponent : public Component
{
public:
    EntityType entityType;

    Vector2 start; // scalar of dims, relative to bottom left of object
    Vector2 end;   // scalar of dims, relative to top right of object

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<RigidbodyComponent> pRigidbody;

    // positions of each corner of collider
    Vector2 bottomLeft; // position of bottom left corner of collider
    Vector2 topRight;   // position of top right corner of collider

    std::set<std::shared_ptr<ColliderComponent>> colsTouching; // colliders that this is touching

    bool borderEnabled[4]{true, true, true, true}; // whether each side of collider is isEnabled or not (for Box only)
                                                   // order: left right bottom top

    bool doCollisions = true;    // if true, detects collisions between other colliders
    bool isTrigger = false;      // if true and if collision detected, doesn't alter position of pTransform

    ColliderComponent();

    // initializes state of collider, returns true if sucessful
    bool init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody, bool doCollisions = true);

    void update(float time); // performs whileTouching on colliders in colsTouching, also updates positions of borders

    void onCollisionEnter(std::shared_ptr<ColliderComponent> pOther);
    void onCollisionExit(std::shared_ptr<ColliderComponent> pOther);
    void whileTouching(std::shared_ptr<ColliderComponent> pOther);
    bool isTouching(std::shared_ptr<ColliderComponent> pOther); // returns true if this is touching other

    void kill() override;
};

enum class HitboxType
{
    Box,
    Circle
};

class HitboxComponent : public Component
{
private:
    static std::map<std::string, HitboxType> _typeFromString;

public:
    EntityType entityType;

    HitboxType hitboxType = HitboxType::Box;

    Vector2 start; // for Box: gives bottom left of hitbox as a scalar of pxDims, relative to top left of sprite
                   // for Circle: acts as center of hitbox as a scalar of pxDims, relative to top left of sprite
    Vector2 end;   // for Box: gives top right of hitbox as a scalar of pxDims, relative to bottom right of sprite
                   // for Circle: gives x and y radius of circle as a scalar of pxDims

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<RigidbodyComponent> pRigidbody;

    // pixel positions of each corner of hitbox (for Box and Circle)
    Vector2Int bottomLeft; // pxPos of bottom left of hitbox
    Vector2Int topRight;   // pxPos of top right of hitbox
    Vector2Int center;     // pxPos of center of hitbox (for Circle only)

    std::set<std::shared_ptr<HitboxComponent>> hbTouching; // hitboxes that this is touching

    HitboxComponent();

    // initializes state of hitbox, returns true if sucessful
    bool init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<RigidbodyComponent> pRigidbody);

    void update(float time); // performs whileTouching on hitboxes in hbTouching, also updates positions of edges

    void onHitboxEnter(std::shared_ptr<HitboxComponent> pOther);
    void onHitboxExit(std::shared_ptr<HitboxComponent> pOther);
    bool isTouching(std::shared_ptr<HitboxComponent> pOther); // returns true if this is touching other

    void kill() override;
};

class RigidbodyComponent : public Component
{
public:
    bool isStatic = false; // doesn't move if static

    Vector2 velocity; // meters/sec
    Vector2 nextPos;  // meters

    bool nextPosSet = false;

    std::shared_ptr<TransformComponent> pTransform;
    std::shared_ptr<ColliderComponent> pCollider;

    RigidbodyComponent();
    bool init(EntityType entityType, std::shared_ptr<TransformComponent> pTransform, std::shared_ptr<ColliderComponent> pCollider = nullptr); // returns true if successful
    void update(float time);
    void kill() override;
};

class HealthComponent : public Component
{
public:
    float baseHealth;
    float health;

    std::shared_ptr<RendererComponent> pRedRenderer;
    std::shared_ptr<TransformComponent> pRedTransform;

    std::shared_ptr<RendererComponent> pGreenRenderer;
    std::shared_ptr<TransformComponent> pGreenTransform;

    std::shared_ptr<TransformComponent> pFollowTransform;
    Vector2 followOffset;

    HealthComponent();
    bool init(float baseHealth, std::shared_ptr<TransformComponent> pFollowTransform, Vector2 followOffset);
    void update(float time);
    void heal(float healAmount);
    bool damage(float dmgAmount); // returns true if dead after damage dealt
    void kill() override;
    void setPos(Vector2 pos);
};

class AnimatorComponent : public Component
{
private:
    AnimationManager &_animationManager;

    std::shared_ptr<Animation> _pAnimation;

    std::shared_ptr<RendererComponent> _pRenderer;
    std::vector<SDL_Rect> _rects;

    int _currFrame;
    int _frameCount;
    float _frameTime;
    float _duration;
    float _timeSinceFrame;
    bool _loops;
    bool _playing;

    EntityType _animationType;
    std::string _animationName;

public:
    AnimatorComponent();
    bool init(std::shared_ptr<RendererComponent> pRenderer);
    void update(float time);
    void setAnimation(EntityType entityType, std::string name);
    void kill();

    inline void startAnimation() { _playing = true; }
    inline void stopAnimation() { _playing = false; }
};

#endif