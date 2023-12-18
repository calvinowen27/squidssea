#ifndef ANIMATION_MANAGER_INCLUDE
#define ANIMATION_MANAGER_INCLUDE

#include "../../nlohmann_json/json.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

using json = nlohmann::json;

class Animation;
enum class EntityType;

class AnimationManager
{
private:
    std::map<EntityType, std::map<std::string, std::shared_ptr<Animation>>> _animations;

    std::string _animationDataPath = "./data/animations.json";

    json _animationData;

public:
    AnimationManager();
    void init();
    void loadAnimations();
    std::shared_ptr<Animation> newAnimation(std::string entityTypeName, std::string name, int animationIdx);
    std::shared_ptr<Animation> getAnimation(EntityType entityType, std::string name);
};

#endif