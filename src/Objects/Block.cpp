#include "../../include/game/Objects/Block.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Objects/GridManager.hpp"
#include "../../include/game/UI/UIPiece.hpp"

Block::Block() : Object(), _gridManager(*game.pGridManager)
{
}

bool Block::init(Vector2 pos)
{
    Object::init(EntityType::Block, pos);

    return true;
}

void Block::setAsGoal()
{
    if (_goal)
    {
        _goal->removeAsGoal();
    }

    _goal = this;

    if (pRenderer)
        pRenderer->setTexture("goal");
}

void Block::removeAsGoal()
{
    _goal = nullptr;

    if (pRenderer)
        pRenderer->setTexture("block");
}

void Block::onCollisionEnter(Entity *pOther)
{
    Entity::onCollisionEnter(pOther);

    if (_goal == this && pOther && pOther->getType() == EntityType::Player)
    {
        chooseNewGoal();
        game.pUIManager->getPieceUI()->reset();
        game.score++;
    }
}

void Block::chooseNewGoal()
{
    GridManager &gridManager = *Game::getInstance()->pGridManager;
    Vector2Int gridDims = gridManager.getGridDims();
    Vector2Int newGoalPos(rand() % gridDims.x, rand() % gridDims.y);

    std::shared_ptr<Block> pBlock = gridManager.getBlockAtPos(newGoalPos);

    if (pBlock)
    {
        pBlock->setAsGoal();
    }
    else
    {
        if (gridManager.placePiece(newGoalPos, PieceType::Bit))
            gridManager.getBlockAtPos(newGoalPos)->setAsGoal();
        else
            chooseNewGoal();
    }
}

void Block::kill()
{
    if (_goal == this)
        chooseNewGoal();

    Object::kill();
}