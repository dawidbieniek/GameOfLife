#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <ncurses.h>

// Creates space for input window
WINDOW* createInputWindow(int x, int y);

// Shows input window
void showInputWindow(WINDOW* window, const char* title);

// Hides input window
void hideInputWindow(WINDOW* window);

// Reads chars form input window until Enter is pressed
char* handleInputWindowInput(WINDOW* window);

// Shows message in title of input window
void showMessage(WINDOW* window, char* message);

// Shows error message in title of input window. Lcoks thread until button is pressed
void showError(WINDOW* window, char* message);
#endif