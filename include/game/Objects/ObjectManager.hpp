#ifndef OBJECT_MANAGER_INCLUDE
#define OBJECT_MANAGER_INCLUDE

#include "../../SDL2/SDL.h"
#include "../../nlohmann_json/json.hpp"
#include "../Util/Vector2.hpp"
#include "../Entity/Registry.hpp"

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <set>

class Game;
class Object;
class Block;

using json = nlohmann::json;

class ObjectManager
{
private:
    static ObjectManager *_pInstance;

    Game &_game;

    std::vector<std::shared_ptr<Object>> _objects;
    std::set<std::shared_ptr<Entity>> _entities;

    std::shared_ptr<Block> **_grid;

    std::string _entityDataPath = "./data/entityData.json";
    json _entityData;

public:
    ObjectManager();
    ~ObjectManager();
    static ObjectManager *getInstance();
    void init();
    void update(float time);
    inline std::set<std::shared_ptr<Entity>> getEntities() { return _entities; }
    json getEntityData(EntityType entityType);

    template <typename T>
    std::shared_ptr<T> newEntity()
    {
        // do not use entity creation function if type is not a entity
        if (!std::is_base_of<Entity, T>::value)
        {
            throw new std::invalid_argument("ObjectManager::newEntity<Type>() : Type must be derived from Entity class.");
        }

        std::shared_ptr<T> entity = TypePool<T>::getInstance()->instantiate();
        std::dynamic_pointer_cast<Entity>(entity);
        _entities.emplace(entity);
        return entity;
    }

    template <typename T>
    void killEntity(std::shared_ptr<T> pEntity)
    {
        // do not use entity kill function if Type is not a entity
        if (!std::is_base_of<Entity, T>::value)
        {
            throw new std::invalid_argument("ObjectManager::killEntity<Type>() : Type must be derived from Entity class.");
        }

        if (pEntity)
        {
            pEntity->kill();
            _entities.erase(pEntity);
            TypePool<T>::getInstance()->release(pEntity);
        }
    }

    template <typename T>
    void killEntitiesOfType()
    {
        // do not use if Type is not derived from Entity
        if (!std::is_base_of<Entity, T>::value)
        {
            throw new std::invalid_argument("ObjectManager::killEntitiesOfType<Type>() : Type must be derived from Entity class.");
        }

        for (auto entity : TypePool<T>::getInstance()->getAlive())
        {
            entity->kill();
        }
    }
};

#endif