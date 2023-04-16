#ifndef BOARD_H
#define BOARD_H

#include <ncurses.h>

typedef struct {
    int* cells;
    int w;
    int h;
} Board;

// Sets all cells of board to 0
void clearBoard(Board* board);

// Creates new board
Board* createBoard(int w, int h);

// Sets cell of board to given value
void setCell(int x, int y, int value, Board* board);

// Returns value of cell in board
int getCell(int x, int y, Board* board);

// TMP
void writeBoard(Board* board);

// Creates new virtual window for board
WINDOW* createBoardWindow(Board* board, int x, int y);

// Draws board on window. Draws window on screen
void updateBoardWindow(Board* board, WINDOW* window);

// Clears memory of board
void destroyBoard(Board* board);
#endif