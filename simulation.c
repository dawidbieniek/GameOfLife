#include "simulation.h"

#include "board.h"
#include "rules.h"

int boardWrapping = 0;

int clampX(int x, Board* board)
{
    if(boardWrapping)
    {
        if(x < 0) return board->w + x;
        if(x >= board->w) return x - board->w;
    }
    else
    {
        if(x < 0) return 0;
        if(x >= board->w) return board->w - 1;
    }

    return x;
}

int clampY(int y, Board* board)
{
    if(boardWrapping)
    {
        if(y < 0) return board->h + y;
        if(y >= board->h) return y - board->h;
    }
    else
    {
        if(y < 0) return 0;
        if(y >= board->h) return board->h - 1;
    }

    return y;
}

int getNeighbourCount(int x, int y, Board* board)
{
    int neighbours = 0;

    for(int j = y - 1; j <= y + 1; j++)
    {
        for(int i = x - 1; i <= x + 1; i++)
        {
            if(i == x && j == y) continue;

            if(getCell(clampX(i, board), clampY(j, board), board))
            {
                neighbours++;
            }
        }
    }

    return neighbours;
}

// NOTE: Using another board for storing neighbours allows to avoid using board buffers and changing pointers
void stepSimulation(Board* board, Ruleset* ruleset)
{
    Board* neighbourBoard = createBoard(board->w, board->h);    // Im using board to store neighbour count
    
    // Count number of neighbours for each cell in original board
    for(int y = 0; y < board->h; y++)
    {
        for(int x = 0; x < board->w; x++)
        {
            setCell(x, y, getNeighbourCount(x, y, board), neighbourBoard);
        }
    }

    // Update board state based on number of neighbours
    for(int y = 0; y < board->h; y++)
    {
        for(int x = 0; x < board->w; x++)
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