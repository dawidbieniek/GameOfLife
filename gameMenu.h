#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <pthread.h>

/* Creates new window for menu */
WINDOW* createMenuWindow(int x, int y);

/* Sets refresh mutex for menu */
void setMenuRefreshMutex(pthread_mutex_t* refreshMutex);

/* Updates menu window display */
void updateMenuWindow();

/* Updates menu window display. Can cause thread racing */
void updateMenuWindowUnsafe();

/* Handles input of arrow keys and space. Returns selected item index. -1 if handling arrows */
int handleMenuInput(int ch);

/* Locks some of menu options if lock = 1; unlocks them if lock = 0; */
void lockMenuOptions(int lock);

/* Checks if option is aviable */
int isMenuOptionAviable(int menuOpt, int simState);

#endif