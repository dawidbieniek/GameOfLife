#include "inputWindow.h"

#include <ncurses.h>

#include <stdlib.h>

WINDOW* createInputWindow(int x, int y)
{
    WINDOW* window;
    
    window = newwin(3, 30, y, x);

    return window;
}

void showInputWindow(WINDOW* window, const char* title)
{
    mvwprintw(window, 0, 0, "%s", title);
    mvwprintw(window, 1, 0, ">");
    wrefresh(window);
}

void hideInputWindow(WINDOW* window)
{
    wclear(window);
    wrefresh(window);
}

char* handleInputWindowInput(WINDOW* window)
{
    char* c;
    int i;
    
    echo();             /* Enables printing of pressed characters in window */
    /* nocbreak();         /* Enables line buffering */

    c = malloc(sizeof(char) * 50);
    wgetnstr(window, c, 50);

    noecho();           /* Disable printing of pressed characters in window */
    cbreak();           /* Disables line buffering */
    hideInputWindow(window);

    return c;
}

void showMessage(WINDOW* window, char* message)
{
    mvwprintw(window, 0, 0, "%s", message);
    wrefresh(window);
}

void showError(WINDOW* window, char* message)
{
    wattron(window, COLOR_PAIR(2) | A_BOLD);
    showMessage(window, message);
    wattroff(window, COLOR_PAIR(2) | A_BOLD);
    wgetch(window);
    hideInputWindow(window);
}