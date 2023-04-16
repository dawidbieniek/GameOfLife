#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <ncurses.h>

// Creates space for input window
WINDOW* createInputWindow(int x, int y);

void showInputWindow(WINDOW* window, const char* title);

void hideInputWindow(WINDOW* window);

char* handleInputWindowInput(WINDOW* window);

#endif