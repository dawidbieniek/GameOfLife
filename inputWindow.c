#include "inputWindow.h"

#include <ncurses.h>
#include <stdlib.h>

WINDOW* inputWindow_inputWindow = NULL;


WINDOW* createInputWindow(int x, int y)
{    
    inputWindow_inputWindow = newwin(3, 25, y, x);

    return inputWindow_inputWindow;
}

void showInputWindow(const char* title)
{
    mvwprintw(inputWindow_inputWindow, 0, 0, "%s", title);
    mvwprintw(inputWindow_inputWindow, 1, 0, ">");
    wrefresh(inputWindow_inputWindow);
}

void hideInputWindow()
{
    werase(inputWindow_inputWindow);
    wrefresh(inputWindow_inputWindow);
}

char* handleInputWindowInput()
{
    char* c;
    int i;
    
    echo();             /* Enables printing of pressed characters in window */
    nocbreak();         /* Enables line buffering */

    c = malloc(sizeof(char) * 50);
    wgetnstr(inputWindow_inputWindow, c, 48);

    noecho();           /* Disable printing of pressed characters in window */
    cbreak();           /* Disables line buffering */
    hideInputWindow();

    return c;
}

void showMessage(WINDOW* window, char* message)
{
    mvwprintw(inputWindow_inputWindow, 0, 0, "%s", message);
    wrefresh(inputWindow_inputWindow);
}

void showError(char* message)
{
    wattron(inputWindow_inputWindow, COLOR_PAIR(2) | A_BOLD);
    showMessage(inputWindow_inputWindow, message);
    wattroff(inputWindow_inputWindow, COLOR_PAIR(2) | A_BOLD);
    wgetch(inputWindow_inputWindow);
    hideInputWindow(inputWindow_inputWindow);
}