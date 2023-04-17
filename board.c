#include "board.h"

#include <ncurses.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

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

void resizeBoard(Board* board, int w, int h)
{
    if(w == board->w && h == board->h) return;

    int* newCells = calloc(w * h, sizeof(int));

    int maxY = board->h > h ? h : board->h;
    int maxX = board->w > w ? w : board->w;

    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            newCells[y * w + x] = getCell(x, y, board);
        }
    }

    free(board->cells);
    board->cells = newCells;
    board->h = h;
    board->w = w;
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
    for(int y = 0; y < board->h; y++)
    {
        for(int x = 0; x < board->w; x++)
        {
            mvprintw(y+40, x, "%c ", getCell(x, y, board) ? 'o' : '.');
        }
    }
    refresh();
}

WINDOW* createBoardWindow(Board* board, int x, int y)
{
#ifdef WIDE_MODE
    return newwin(board->h+2, (board->w*2)+2, y, x);
#else
    return newwin(board->h+2, board->w+2, y, x);
#endif
}

void resizeBoardWindow(WINDOW* window, int w, int h)
{
    werase(window);
    wrefresh(window);
#ifdef WIDE_MODE
    wresize(window, h+2, (w*2)+2);   // +2 for border
#else
    wresize(window, h+2, w+2);   // +2 for border
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

int saveBoard(Board* board, char* path)
{
    FILE* file = fopen(path, "w");
    if(file == NULL)
    {
        return 0;
    }

    fprintf(file, "%d %d\n", board->w, board->h);

    for(int i = 0; i < board->h; i++)
    {
        for(int x = 0; x < board->w; x++)
        {
            fputc(getCell(x, i, board) + '0', file);
        }
        fputc('\n', file);
    }

    fclose(file);

    return 1;
}

int loadBoard(Board* board, char* path)
{
    FILE* file = fopen(path, "r");
    if(file == NULL)
    {
        return 0;
    }
    
    char* line;

    int w, h;
    fscanf(file, "%d %d\n", &w, &h);

    if(w < 1 || w > 50 || h < 1 || h > 50)
    {
        fclose(file);
        return 0;
    }

    int* newCells = malloc(sizeof(int) * 400);

    int ch;
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            ch = fgetc(file);
            newCells[y * w + x] = ch - '0';
        }
        fgetc(file);    // Ignore newline char
    }

    free(board->cells);
    board->cells = newCells;
    board->h = h;
    board->w = w;

    fclose(file);

    return 1;
}

void destroyBoard(Board* board)
{
    free(board->cells);
    free(board);
    board = NULL;
}