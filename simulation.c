#include "simulation.h"

#include "board.h"
#include "rules.h"

int boardWrapping = 0;

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

int clampXWrap(int x, Board* board)
{
    if(x < 0) return board->w + x;
    if(x >= board->w) return x - board->w;

    return x;
}

int clampYWrap(int y, Board* board)
{

    if(y < 0) return board->h + y;
    if(y >= board->h) return y - board->h;
        
    return y;
}

int getNeighbourCount(int x, int y, Board* board)
{
    int neighbours, i, j;

    neighbours = 0;
    for(j = clampY(y - 1, board); j <= clampY(y + 1, board); j++)
    {
        for(i = clampX(x - 1, board); i <= clampX(x + 1, board); i++)
        {
            if(i == x && j == y) continue;

            neighbours += getCell(i, j, board);
        }
    }

    return neighbours;
}

int getNeighbourCountWrap(int x, int y, Board* board)
{
    int neighbours, j, i;

    neighbours = 0;
    for(j = y - 1; j <= y + 1; j++)
    {
        for(i = x - 1; i <= x + 1; i++)
        {
            if(i == x && j == y) continue;

            neighbours += getCell(clampXWrap(i, board), clampYWrap(j, board), board);
        }
    }

    return neighbours;
}

/* NOTE: Using another board for storing neighbours allows to avoid using board buffers and changing pointers */
void stepSimulation(Board* board, Ruleset* ruleset)
{
    Board* neighbourBoard;
    int y, x;

    neighbourBoard = createBoard(board->w, board->h);    /* Im using board to store neighbour count */
    
    /* Count number of neighbours for each cell in original board */
    if(boardWrapping)
    {    
        for(y = 0; y < board->h; y++)
        {
            for(x = 0; x < board->w; x++)
            {
                setCell(x, y, getNeighbourCountWrap(x, y, board), neighbourBoard);
            }
        }
    }
    else
    {    
        for(y = 0; y < board->h; y++)
        {
            for(x = 0; x < board->w; x++)
            {
                setCell(x, y, getNeighbourCount(x, y, board), neighbourBoard);
            }
        }
    }

    /* Update board state based on number of neighbours */
    for(y = 0; y < board->h; y++)
    {
        for(x = 0; x < board->w; x++)
        {
            setCell(x, y, 
                nextCellState(ruleset, 
                    getCell(x, y, board), 
                    getCell(x, y, neighbourBoard)), 
                board);
        }
    }

    destroyBoard(neighbourBoard);
}

void setWrapping(int value)
{
    boardWrapping = value;
}