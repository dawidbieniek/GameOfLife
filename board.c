#include "board.h"

#include <ncurses.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void clearBoard(Board* board)
{
    int i;

    for(i = 0; i < board->w * board->h; i++)
    {
        board->cells[i] = 0;
    }
}

Board* createBoard(int w, int h)
{
    Board* board;

    board = malloc(sizeof(Board));
    board->cells = calloc(w * h, sizeof(int));
    board->w = w;
    board->h = h;

    return board;
}

void resizeBoard(Board* board, int w, int h)
{
    int* newCells;
    int maxY, maxX, y, x;

    if(w == board->w && h == board->h) return;

    newCells = calloc(w * h, sizeof(int));

    maxY = board->h > h ? h : board->h;
    maxX = board->w > w ? w : board->w;

    for(y = 0; y < maxY; y++)
    {
        for(x = 0; x < maxX; x++)
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
    wresize(window, h+2, (w*2)+2);   /* +2 for border */
#else
    wresize(window, h+2, w+2);   /* +2 for border */
#endif
}

/* TODO: Pozbyć się zwracanego WINDOW, wszystko przechować globalnie w tym pliku */
void updateBoardWindow(Board* board, WINDOW* window, int selectedX, int selectedY)
{
    int y, x;

    for(y = 0; y < board->h; y++)
    {
        wmove(window, y+1, 1);
        for(x = 0; x < board->w; x++)
        {
            if(x == selectedX && y == selectedY)
            {
                wattron(window, COLOR_PAIR(3) | A_BOLD);
            }

            wprintw(window, "%c", getCell(x, y, board) ? 'o' : '.');
            
            if(x == selectedX && y == selectedY)
            {
                wattroff(window, COLOR_PAIR(3) | A_BOLD);
            }
#ifdef WIDE_MODE
            wprintw(window, " ");
#endif
    
        }
    }
    box(window, 0, 0);
    wrefresh(window);
}

int saveBoard(Board* board, char* path)
{
    FILE* file;
    int i, x;

    file = fopen(path, "w");
    if(file == NULL)
    {
        return 0;
    }

    fprintf(file, "%d %d\n", board->w, board->h);

    for(i = 0; i < board->h; i++)
    {
        for(x = 0; x < board->w; x++)
        {
            fputc(getCell(x, i, board) + '0', file);
        }
        fputc('\n', file);
    }

    fclose(file);

    return 1;
}

int loadBoard(Board* board, char* path, int maxW, int maxH)
{
    FILE* file;
    int w, h, ch, y, x;
    int* newCells;

    file = fopen(path, "r");
    if(file == NULL)
    {
        return 0;
    }
    
    fscanf(file, "%d %d\n", &w, &h);

    if(w < 1 || w > maxW || h < 1 || h > maxH)
    {
        fclose(file);
        return 0;
    }

    newCells = malloc(sizeof(int) * 400);

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            ch = fgetc(file);
            newCells[y * w + x] = ch - '0';
        }
        fgetc(file);    /* Ignore newline char */
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