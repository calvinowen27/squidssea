#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Objects/Object.hpp"

#include <fstream>
#include <type_traits>

ObjectManager *ObjectManager::_pInstance;

ObjectManager::ObjectManager() : _game(*Game::getInstance())
{
}

ObjectManager::~ObjectManager()
{
}

ObjectManager *ObjectManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new ObjectManager();
    }

    return _pInstance;
}

void ObjectManager::init()
{
    std::ifstream f(_entityDataPath);
    _entityData = json::parse(f);
    f.close();
}

void ObjectManager::update(float time)
{
    // obj.update();
    std::vector<std::shared_ptr<Entity>> deadEntities;

    for (auto entity : _entities)
    {
        if (entity->isAlive())
            entity->update(time);
        else
            deadEntities.push_back(entity);
    }

    for (auto entity : deadEntities)
        killEntity(entity);
}

json ObjectManager::getEntityData(EntityType entityType)
{
    std::string typeString = Entity::getStringFromType(entityType);
    if(!_entityData.contains(typeString))
    {
        throw new std::invalid_argument("ObjectManager::getEntityData(entityType): entityType '" + typeString + "' not in entityData.json file.\n");        
    }

    return _entityData[typeString];
}