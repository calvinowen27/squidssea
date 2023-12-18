#include "../../include/game/Animation/AnimationManager.hpp"
#include "../../include/game/Animation/Animation.hpp"
#include "../../include/game/Entity/Entity.hpp"

#include <fstream>

AnimationManager::AnimationManager()
{
}

void AnimationManager::init()
{
    std::ifstream f(_animationDataPath);
    _animationData = json::parse(f);
    f.close();

    loadAnimations();
}

void AnimationManager::loadAnimations()
{
    uint32_t idx;
    EntityType entityType;

    for (auto &[typeName, typeAnimations] : _animationData.items())
    {
        entityType = Entity::getTypeFromString(typeName);
        idx = 0;
        for (auto &animData : typeAnimations)
        {
            std::shared_ptr<Animation> animation = std::make_shared<Animation>(this);
            animation->init(entityType, animData["name"], (float)animData["duration"], (int)animData["frameCount"], idx, animData["loops"]);
            _animations[entityType][animData["name"]] = animation;

            idx++;
        }
    }
}

std::shared_ptr<Animation> AnimationManager::newAnimation(std::string entityTypeName, std::string name, int animationIdx)
{
    EntityType entityType = Entity::getTypeFromString(entityTypeName);

    if (!_animationData.contains(entityTypeName))
    {
        std::cerr << "AnimationManager::newAnimation(entityType): Invalid entityType '" << entityTypeName << "'. Returning nullptr." << std::endl;
        return nullptr;
    }

    float duration = (float)_animationData[entityTypeName][animationIdx][name]["duration"];
    int frameCount = (int)_animationData[entityTypeName][animationIdx][name]["frameCount"];

    std::shared_ptr<Animation> animation = std::make_shared<Animation>(this);
    animation->init(entityType, name, duration, frameCount, animationIdx, _animationData[entityTypeName][animationIdx][name]["loops"]);
    _animations[entityType][name] = animation;

    return animation;
}

std::shared_ptr<Animation> AnimationManager::getAnimation(EntityType entityType, std::string name)
{
    if (_animations[entityType].find(name) == _animations[entityType].end())
    {
        std::cerr << "AnimationManager::getAnimation(entityType, name) : Could not find name \"" << name << "\" in _animations[entityType]. entityType: " << Entity::getStringFromType(entityType) << ". No animation with this name exists.\n";
        return nullptr;
    }

    return _animations[entityType][name];
}