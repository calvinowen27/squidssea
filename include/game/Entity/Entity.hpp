#ifndef ENTITY_INCLUDE
#define ENTITY_INCLUDE

#include "Registry.hpp"
#include "../Util/Vector2.hpp"

#include <string>
#include <map>

enum class EntityType
{
    Player,
    Border,
    Block,
    NotSet
};

class Game;
class ContentManager;
class Registry;
class ObjectManager;
class LevelManager;

class Entity
{
private:
    static std::map<EntityType, std::string> _stringFromType;
    static std::map<std::string, EntityType> _typeFromString;

public:
    EntityType type = EntityType::NotSet;

protected:
    Game &game;
    ObjectManager &objectManager;
    ContentManager &contentManager;
    Registry &registry;

    // EntityType type;

    std::shared_ptr<RendererComponent> pRenderer;
    std::shared_ptr<TransformComponent> pTransform;

    bool alive;

public:
    Entity();
    virtual bool init(EntityType type, Vector2 pos, Vector2 dims = Vector2(1, 1));
    virtual void update(float time);
    void queueKill();
    virtual void kill();

    virtual void onCollisionEnter(Entity *pOther);
    virtual void onCollisionExit(Entity *pOther);
    virtual void onHitboxEnter(Entity *pOther);
    virtual void onHitboxExit(Entity *pOther);
    virtual void whileTouching(Entity *pOther);

    inline EntityType getType() { return type; }
    inline Vector2 getPos() { return pTransform->pos; }
    inline Vector2Int getPxPos()
    {
        pTransform->updatePxPos();
        return pTransform->pxPos;
    }
    inline Vector2 getDims() { return pTransform->dims; }
    inline Vector2Int getPxDims() { return pTransform->pxDims; }
    inline bool isAlive() { return alive; }
    inline static std::string getStringFromType(EntityType entityType) { return _stringFromType[entityType]; }
    inline static EntityType getTypeFromString(std::string typeName) { return _typeFromString[typeName]; }
};

#endif