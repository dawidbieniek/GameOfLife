#include "simulation.h"

#include "board.h"
#include "rules.h"

int clampX(int x, Board* board)
{
    if(x < 0) return 0;
    if(x >= board->w) return board->w - 1;

    return x;
}

int clampY(int y, Board* board)
{
    if(y < 0) return 0;
    if(y >= board->h) return board->h - 1;

    return y;
}

int getNeighbourCount(int x, int y, Board* board)
{
    int neighbours = 0;

    for(int j = clampY(y - 1, board); j <= clampY(y + 1, board); j++)
    {
        for(int i = clampX(x - 1, board); i <= clampX(x + 1, board); i++)
        {
            if(i == x && j == y) continue;

            if(getCell(i, j, board))
            {
                neighbours++;
            }
        }
    }

    return neighbours;
}

Board* stepSimulation(Board* board, Ruleset* ruleset)
{
    Board* newBoard = createBoard(board->w, board->h);

    for(int y = 0; y < board->h; y++)
    {
        for(int x = 0; x < board->w; x++)
        {
            setCell(x, y, 
                nextCellState(ruleset, 
                    getCell(x, y, board), 
                    getNeighbourCount(x, y, board)), 
                newBoard);
        }
    }

    destroyBoard(board);

    return newBoard;
}