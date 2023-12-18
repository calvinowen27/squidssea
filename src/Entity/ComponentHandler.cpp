#include "../../include/game/Entity/ComponentHandler.hpp"
#include "../../include/game/Entity/Registry.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Entity/Components.hpp"
#include "../../include/game/Entity/Entity.hpp"

#include <algorithm>

ComponentHandler::ComponentHandler() : _game(*Game::getInstance()), _registry(*_game.pRegistry)
{
}

void ComponentHandler::draw(SDL_Renderer *pRenderer)
{
    auto rendererSet = _registry.getComponents<RendererComponent>();
    auto renderers = std::vector<std::shared_ptr<RendererComponent>>(rendererSet.begin(), rendererSet.end());
    std::sort(renderers.begin(), renderers.end(), rendererComparator);

    for (auto renderer : renderers)
    {
        if (renderer && renderer->getIsEnabled())
            renderer->draw(pRenderer);
    }
}

void ComponentHandler::update(float time)
{
    updateColliders(time);
    updateRigidbodies(time);
    updateHealthComponents(time);
    updateTransforms(time);
    updateHitboxes(time);
    updateAnimators(time);
}

void ComponentHandler::updateColliders(float time)
{
    auto colliders = _registry.getComponents<ColliderComponent>();
    for (auto pCol : colliders)
    {
        if (!pCol->getIsEnabled() || pCol->pRigidbody->isStatic)
            continue;

        pCol->update(time);

        Vector2 deltaPos = pCol->pRigidbody->velocity * time;
        Vector2 nextPos = pCol->pTransform->pos + deltaPos;
        Vector2 startNextPos = nextPos;

        for (auto pOther : colliders)
        {
            if (!pOther->getIsEnabled() || !pCol->getIsEnabled())
                continue;

            if (pCol == pOther)
                continue;

            if (!pCol->doCollisions || !pOther->doCollisions)
                continue;

            // skip if not touching
            if (pCol->bottomLeft.x + deltaPos.x >= pOther->topRight.x || pCol->topRight.x + deltaPos.x <= pOther->bottomLeft.x ||
                pCol->bottomLeft.y + deltaPos.y >= pOther->topRight.y || pCol->topRight.y + deltaPos.y <= pOther->bottomLeft.y)
            {
                if (pCol->isTouching(pOther) || pOther->isTouching(pCol))
                {
                    pCol->onCollisionExit(pOther);
                    pOther->onCollisionExit(pCol);
                }

                continue;
            }
            else
            {
                nextPos = handleCollision(pCol, pOther, nextPos, deltaPos);
            }
        }

        if(startNextPos != nextPos)
            pCol->pRigidbody->velocity = (nextPos - pCol->pTransform->pos) / time;
            // pCol->pRigidbody->nextPos = nextPos;
    }
}

Vector2 ComponentHandler::handleCollision(std::shared_ptr<ColliderComponent> pCol1, std::shared_ptr<ColliderComponent> pCol2, Vector2 nextPos, Vector2 dPos)
{
    // this left and other right
    if (pCol1->borderEnabled[0] && pCol2->borderEnabled[1])
    {
        if (pCol1->bottomLeft.x >= pCol2->topRight.x && pCol1->bottomLeft.x + dPos.x < pCol2->topRight.x)
        {
            if (!(pCol1->isTrigger || pCol2->isTrigger))
                nextPos.x = pCol2->topRight.x - (pCol1->start.x * pCol1->pTransform->dims.x);

            pCol1->onCollisionEnter(pCol2);
            pCol2->onCollisionEnter(pCol1);
        }
    }

    // this right and other left
    if (pCol1->borderEnabled[1] && pCol2->borderEnabled[0])
    {
        if (pCol1->topRight.x <= pCol2->bottomLeft.x && pCol1->topRight.x + dPos.x > pCol2->bottomLeft.x)
        {
            if (!(pCol1->isTrigger || pCol2->isTrigger))
                nextPos.x = pCol2->bottomLeft.x - (pCol1->end.x * pCol1->pTransform->dims.x);

            pCol1->onCollisionEnter(pCol2);
            pCol2->onCollisionEnter(pCol1);
        }
    }

    // this bottom and other top
    if (pCol1->borderEnabled[2] && pCol2->borderEnabled[3])
    {
        if (pCol1->bottomLeft.y >= pCol2->topRight.y && pCol1->bottomLeft.y + dPos.y < pCol2->topRight.y)
        {
            if (!(pCol1->isTrigger || pCol2->isTrigger))
                nextPos.y = pCol2->topRight.y - (pCol1->start.y * pCol1->pTransform->dims.y);

            pCol1->onCollisionEnter(pCol2);
            pCol2->onCollisionEnter(pCol1);
        }
    }

    // this top and other bottom
    if (pCol1->borderEnabled[3] && pCol2->borderEnabled[2])
    {
        if (pCol1->topRight.y <= pCol2->bottomLeft.y && pCol1->topRight.y + dPos.y > pCol2->bottomLeft.y)
        {
            if (!(pCol1->isTrigger || pCol2->isTrigger))
                nextPos.y = pCol2->bottomLeft.y - (pCol1->end.y * pCol1->pTransform->dims.y);

            pCol1->onCollisionEnter(pCol2);
            pCol2->onCollisionEnter(pCol1);
        }
    }

    return nextPos;
}

void ComponentHandler::updateRigidbodies(float time)
{
    auto rigidbodies = _registry.getComponents<RigidbodyComponent>();
    for (auto pRb : rigidbodies)
    {
        pRb->update(time);
    }
}

void ComponentHandler::updateHealthComponents(float time)
{
    auto healthComponents = _registry.getComponents<HealthComponent>();
    for (auto pHC : healthComponents)
    {
        pHC->update(time);
    }
}

void ComponentHandler::updateTransforms(float time)
{
    auto transforms = _registry.getComponents<TransformComponent>();
    for (auto pTransform : transforms)
    {
        pTransform->update(time);
    }
}

void ComponentHandler::updateHitboxes(float time)
{
    auto hitboxes = _registry.getComponents<HitboxComponent>();
    for (auto pHb1 : hitboxes)
    {
        pHb1->update(time);

        for (auto pHb2 : hitboxes)
        {
            if (!pHb2->getIsEnabled() || !pHb1->getIsEnabled())
                continue;

            if (pHb1 == pHb2)
                continue;

            // Box-Box
            if (pHb1->hitboxType == HitboxType::Box && pHb2->hitboxType == HitboxType::Box)
            {
                // skip if not touching
                if (pHb1->bottomLeft.x > pHb2->topRight.x || pHb1->topRight.x < pHb2->bottomLeft.x ||
                    pHb1->bottomLeft.y < pHb2->topRight.y || pHb1->topRight.y > pHb2->bottomLeft.y) // y comparison inverted because comparing pixel positions
                {
                    if (pHb1->isTouching(pHb2) || pHb2->isTouching(pHb1))
                    {
                        pHb1->onHitboxExit(pHb2);
                    }
                    continue;
                }

                // if we make it here there is a collision
                pHb1->onHitboxEnter(pHb2);
                continue;
            }

            // Circle-Box
            if (pHb1->hitboxType == HitboxType::Circle && pHb2->hitboxType == HitboxType::Box)
            {
                int radius = pHb1->end.x * pHb1->pTransform->pxDims.x;
                int radiusSq = radius * radius;

                // check if center of circle lies in box
                if (pHb1->center.x > pHb2->topRight.x || pHb1->center.x < pHb2->bottomLeft.x ||
                    pHb1->center.y < pHb2->topRight.y || pHb1->center.y > pHb2->bottomLeft.y)
                {
                    // center of circle lies outside of box
                    // check if any rectangle edge is touching circle
                    if ((abs(pHb1->center.x - pHb2->topRight.x) <= radius && pHb1->center.y >= pHb2->topRight.y && pHb1->center.y <= pHb2->bottomLeft.y) ||
                        (abs(pHb2->bottomLeft.x - pHb1->center.x) <= radius && pHb1->center.y >= pHb2->topRight.y && pHb1->center.y <= pHb2->bottomLeft.y) ||
                        (abs(pHb1->center.y - pHb2->topRight.y) <= radius && pHb1->center.x >= pHb2->bottomLeft.x && pHb1->center.x <= pHb2->topRight.x) ||
                        (abs(pHb2->bottomLeft.y - pHb1->center.y) <= radius && pHb1->center.x >= pHb2->bottomLeft.x && pHb1->center.x <= pHb2->topRight.x))
                    {
                        // if we make it here there is a collision
                        pHb1->onHitboxEnter(pHb2);
                        continue;
                    }
                    // check distance from circle center to each corner of box
                    else if (Vector2Int::distance(pHb1->center, pHb2->bottomLeft) <= radius || Vector2Int::distance(pHb1->center, Vector2Int(pHb2->bottomLeft.x, pHb2->topRight.y)) <= radius ||
                             Vector2Int::distance(pHb1->center, pHb2->topRight) <= radius || Vector2Int::distance(pHb1->center, Vector2Int(pHb2->topRight.x, pHb2->bottomLeft.y)) <= radius)
                    {
                        // if we make it here there is a collision
                        pHb1->onHitboxEnter(pHb2);
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    // if we make it here there is a collision
                    pHb1->onHitboxEnter(pHb2);
                    continue;
                }
            }

            // Circle-Circle
            if (pHb1->hitboxType == HitboxType::Circle && pHb2->hitboxType == HitboxType::Circle)
            {
                if (Vector2Int::distance(pHb1->center, pHb2->center) <= (pHb1->end.x * pHb1->pTransform->pxDims.x) + (pHb2->end.x * pHb2->pTransform->pxDims.x))
                {
                    pHb1->onHitboxEnter(pHb2);
                }
            }
        }
    }
}

void ComponentHandler::updateAnimators(float time)
{
    auto animators = _registry.getComponents<AnimatorComponent>();
    for (auto animator : animators)
    {
        animator->update(time);
    }
}

bool rendererComparator(std::shared_ptr<RendererComponent> a, std::shared_ptr<RendererComponent> b)
{
    if (!a->getIsEnabled() || !a->pTransform)
        return false;

    if (!b->getIsEnabled() || !b->pTransform)
        return false;

    if (a->renderOrder != b->renderOrder)
        return a->renderOrder > b->renderOrder;

    return a->pTransform->pos.y > b->pTransform->pos.y;
}