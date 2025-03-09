#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <ncurses.h>

/* Creates space for input window */
WINDOW* createInputWindow(int x, int y);

/* Shows input window */
void showInputWindow(const char* title);

/* Hides input window */
void hideInputWindow();

/* Reads chars form input window until Enter is pressed */
char* handleInputWindowInput();

/* Shows error message in title of input window. Lcoks thread until button is pressed */
void showError(char* message);
#endif