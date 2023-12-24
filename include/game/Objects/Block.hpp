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

    bool _isGoal = false;

public:
    Block();
    bool init(Vector2 pos);
    void setAsGoal();
    void removeAsGoal();
    bool isGoal();
    void onCollisionEnter(Entity *pOther) override;
    static void chooseNewGoal();
    static Block *getGoal();
    void kill() override;
};

#endif