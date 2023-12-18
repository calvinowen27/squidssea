#include "../../include/game/Objects/GridManager.hpp"
#include "../../include/game/UI/UIPiece.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Objects/ObjectManager.hpp"
#include "../../include/game/Objects/Block.hpp"
#include "../../include/game/Entity/Player.hpp"

std::shared_ptr<Block> **_grid;

GridManager::GridManager() : _game(*Game::getInstance()), _objectManager(*_game.pObjectManager)
{
}

GridManager::~GridManager()
{
    int x, y;
    for (x = 0; x < _gridDims.x; x++)
    {
        for (y = 0; y < _gridDims.y; y++)
        {
            _grid[x][y] = nullptr;
        }

        delete _grid[x];
    }

    delete _grid;
}

void GridManager::init(Vector2Int gridDims)
{
    _gridDims = gridDims;

    _grid = new std::shared_ptr<Block> *[_gridDims.y];

    int r, c;
    for (r = 0; r < _gridDims.y; r++)
    {
        _grid[r] = new std::shared_ptr<Block>[_gridDims.x];

        for (c = 0; c < _gridDims.x; c++)
        {
            _grid[r][c] = nullptr;
        }
    }

    Block::chooseNewGoal();
}

bool GridManager::placePiece(Vector2Int cellPos, PieceType piece)
{
    if (cellPos.x < 0 || cellPos.y < 0 || cellPos.x >= _gridDims.x || cellPos.y >= _gridDims.y)
        return false;

    if (_grid[cellPos.y][cellPos.x])
        return false;

    int i = 0b100000000;

    Vector2Int d(0, 0);
    int r, c;
    for (c = 0; c < 3; c++)
    {
        d.y = 0;
        for (r = 0; r < 3; r++)
        {
            if ((int)piece & i)
            {
                Vector2Int placePos = cellPos + d;
                if ((_game.pPlayer && (Vector2Int)_game.pPlayer->getPos() == placePos) || 
                    placePos.x >= _gridDims.x || placePos.y >= _gridDims.y || 
                    _grid[placePos.y][placePos.x])
                    return false;
            }

            i >>= 1;
            d.y++;
        }

        d.x++;
    }

    i = 0b100000000;

    d = Vector2Int(0, 0);

    for (c = 0; c < 3; c++)
    {
        d.y = 0;

        for (r = 0; r < 3; r++)
        {
            if ((int)piece & i)
            {
                Vector2Int placePos = cellPos + d;
                _grid[placePos.y][placePos.x] = _objectManager.newEntity<Block>();
                _grid[placePos.y][placePos.x]->init((Vector2)placePos);
            }
            d.y++;

            i >>= 1;
        }

        d.x++;
    }

    checkMatches();

    return true;
}

void GridManager::checkMatches()
{
    int r, c;
    bool match;

    std::vector<int> rowMatches;
    std::vector<int> colMatches;

    for (r = 0; r < _gridDims.y; r++)
    {
        match = true;
        for (c = 0; c < _gridDims.x; c++)
        {
            if (!_grid[r][c])
            {
                match = false;
                break;
            }
        }

        if (match)
            rowMatches.push_back(r);
    }

    for (c = 0; c < _gridDims.x; c++)
    {
        match = true;
        for (r = 0; r < _gridDims.y; r++)
        {
            if (!_grid[r][c])
            {
                match = false;
                break;
            }
        }

        if (match)
            colMatches.push_back(c);
    }

    for (auto row : rowMatches)
    {
        clearRow(row);
    }

    for (auto col : colMatches)
    {
        clearCol(col);
    }
}

void GridManager::clearRow(int row)
{
    std::shared_ptr<Block> pBlock;
    for (int c = 0; c < _gridDims.x; c++)
    {
        pBlock = _grid[row][c];
        if (pBlock && pBlock->isAlive())
        {
            pBlock->queueKill();
            _grid[row][c] = nullptr;
        }
    }
}

void GridManager::clearCol(int col)
{
    std::shared_ptr<Block> pBlock;

    for (int r = 0; r < _gridDims.y; r++)
    {
        pBlock = _grid[r][col];
        if (pBlock && pBlock->isAlive())
        {
            pBlock->queueKill();
            _grid[r][col] = nullptr;
        }
    }
}

void GridManager::clearGrid()
{
    for(int r = 0; r < _gridDims.y; r++)
    {
        clearRow(r);
    }
}

std::shared_ptr<Block> **GridManager::getGrid()
{
    return _grid;
}

Vector2Int GridManager::getGridDims()
{
    return _gridDims;
}

bool GridManager::isBlockAtPos(Vector2Int pos)
{
    return (bool)_grid[pos.y][pos.x];
}

std::shared_ptr<Block> GridManager::getBlockAtPos(Vector2Int pos)
{
    return _grid[pos.y][pos.x];
}