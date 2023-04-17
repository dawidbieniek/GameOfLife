#ifndef MENU_H
#define MENU_H

#include <ncurses.h>

// Creates new window for menu
WINDOW* createMenuWindow();

// Updated menu window display
void updateMenuWindow(WINDOW* window);

// Handles input of arrow keys and space. Returns selected item index. -1 if handling arrows
int handleMenuInput(WINDOW* window, int ch);

// Locks some of menu options if lock = 1; unlocks them if lock = 0;
void lockMenuOptions(WINDOW* window, int lock);

// Checks if option is aviable
int isMenuOptionAviable(int menuOpt, int simState);

#endif