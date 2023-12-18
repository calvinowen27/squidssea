#ifndef BLOCK_INCLUDE
#define BLOCK_INCLUDE

#include "Object.hpp"

#include <memory>

class GridManager;

class Block : public Object
{
private:
    inline static Block *_goal;

    GridManager &_gridManager;

public:
    Block();
    bool init(Vector2 pos);
    void setAsGoal();
    void removeAsGoal();
    void onCollisionEnter(Entity *pOther) override;
    static void chooseNewGoal();
    void kill() override;
};

#endif