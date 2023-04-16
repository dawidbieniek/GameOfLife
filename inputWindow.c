#include "inputWindow.h"

#include <ncurses.h>

#include <stdlib.h>

WINDOW* createInputWindow(int x, int y)
{
    WINDOW* window = newwin(3, 30, y, x);

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
    nodelay(stdscr, 0); // Enables lock for getch()
    echo();             // Enables printing of pressed characters in window
    nocbreak();         // Enables line buffering

    char* c = malloc(sizeof(char) * 50);
    wgetnstr(window, c, 50);

    nodelay(stdscr, 1); // Disables lock for getch()
    noecho();           // Disable printing of pressed characters in window
    cbreak();           // Disables line buffering
    hideInputWindow(window);

    return c;
}