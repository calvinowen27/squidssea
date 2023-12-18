#include "../../include/game/Entity/Entity.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Input/MouseHandler.hpp"

std::map<EntityType, std::string> Entity::_stringFromType = {
    {EntityType::Player, "Player"},
    {EntityType::Border, "Border"},
    {EntityType::Block, "Block"},
    {EntityType::NotSet, "NotSet"}};

std::map<std::string, EntityType> Entity::_typeFromString = {
    {"Player", EntityType::Player},
    {"Border", EntityType::Border},
    {"Block", EntityType::Block},
    {"NotSet", EntityType::NotSet}};

Entity::Entity() : game(*Game::getInstance()), registry(*game.pRegistry), contentManager(*game.pContentManager), objectManager(*game.pObjectManager)
{
}

bool Entity::init(EntityType type, Vector2 pos, Vector2 dims)
{
    this->type = type;
    alive = true;

    pTransform = registry.newComponent<TransformComponent>();
    pRenderer = registry.newComponent<RendererComponent>();

    pTransform->init(type, pos, dims);
    pTransform->setEntity(this);

    pRenderer->init(type, pTransform);
    pRenderer->enable();

    return true;
}

void Entity::update(float time)
{
}

void Entity::queueKill()
{
    alive = false;
}

void Entity::kill()
{
    registry.killComponent(pTransform);
    registry.killComponent(pRenderer);
}

void Entity::onCollisionEnter(Entity *pOther)
{
}

void Entity::onCollisionExit(Entity *pOther)
{
}

void Entity::onHitboxEnter(Entity *pOther)
{
}

void Entity::onHitboxExit(Entity *pOther)
{
}

void Entity::whileTouching(Entity *pOther)
{
}