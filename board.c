#include "board.h"

#include <ncurses.h>
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
    board->cells = calloc(w * h, sizeof(int));
    board->w = w;
    board->h = h;

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
            printf("%c ", getCell(x, y, board) ? 'o' : '.');
        }
        printf("\n");
    }
    
}

WINDOW* createBoardWindow(Board* board, int x, int y)
{
#ifdef WIDE_MODE
    return newwin(board->h+2, (board->w*2)+2, y, x);
#else
    return newwin(board->h+2, board->w+2, y, x);
#endif
}

// TODO: Pozbyć się zwracanego WINDOW, wszystko przechować globalnie w tym pliku
void updateBoardWindow(Board* board, WINDOW* window)
{
    for(int y = 0; y < board->h; y++)
    {
        wmove(window, y+1, 1);
        for(int x = 0; x < board->w; x++)
        {
#ifdef WIDE_MODE
            wprintw(window, "%c ", getCell(x, y, board) ? 'o' : '.');
#else
            wprintw(window, "%c", getCell(x, y, board) ? 'o' : '.');
#endif
        }
    }
    box(window, 0, 0);
    wrefresh(window);
}

void destroyBoard(Board* board)
{
    free(board->cells);
    free(board);
    board = NULL;
}