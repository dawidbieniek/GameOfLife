#include "board.h"

#include <stdlib.h>
#include <stdio.h>


void clearBoard(Board* board)
{
    for(int i = 0; i < board->w * board->h; i++)
    {
        board->cells[i] = 0;
    }
}

Board* createBoard(int w, int h)
{
    Board* board = malloc(sizeof(Board));
    board->cells = malloc(sizeof(int) * w * h);
    board->w = w;
    board->h = h;

    clearBoard(board);

    return board;
}

void setCell(int x, int y, int value, Board* board)
{
    board->cells[y * board->w + x] = value;
}

int getCell(int x, int y, Board* board)
{
    return board->cells[y * board->w + x];
}

void writeBoard(Board* board)
{
    printf("%d x %d\n", board->w, board->h);
    for(int y = 0; y < board->h; y++)
    {
        for(int x = 0; x < board->w; x++)
        {
            printf("%d ", getCell(x, y, board));
        }
        printf("\n");
    }
    
}
// WINDOW* createBoardWindow(Board* board)
// {

// }

// void updateBoardWindow(Board* board, WINDOW* window)
// {

// }

void destroyBoard(Board* board)
{
    free(board->cells);
    free(board);
    board = NULL;
}