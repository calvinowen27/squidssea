#ifndef GRID_MANAGER_INCLUDE
#define GRID_MANAGER_INCLUDE

#include "../Util/Vector2.hpp"
#include <memory>

class Game;
class ObjectManager;
class UIPiece;
enum class PieceType;
class Block;

class GridManager
{
private:
    Game &_game;
    ObjectManager &_objectManager;

    Vector2Int _gridDims;

    std::shared_ptr<Block> **_grid;

public:
    GridManager();
    ~GridManager();
    void init(Vector2Int gridDims);
    bool placePiece(Vector2Int cellPos, PieceType piece);
    void checkMatches();
    void clearRow(int row);
    void clearCol(int col);
    void clearGrid();

    std::shared_ptr<Block>  **getGrid();
    Vector2Int getGridDims();

    bool isBlockAtPos(Vector2Int pos);
    std::shared_ptr<Block> getBlockAtPos(Vector2Int pos);
};

#endif