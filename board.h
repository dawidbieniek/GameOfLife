#ifndef BOARD_H
#define BOARD_H

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

// Clears memory of board
void destroyBoard(Board* board);
#endif