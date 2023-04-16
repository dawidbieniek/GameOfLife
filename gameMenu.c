#include "gameMenu.h"

#include <ncurses.h>

#include <stdlib.h>
#include <menu.h>

MENU* menu;
int selectedItemIndex = 0;

WINDOW* createMenuWindow(int x, int y)
{
    WINDOW* menuWindow= newwin(13, 30, y, x);

    ITEM** items = calloc(14, sizeof(ITEM*));
    items[0] = new_item(" ", "Start [p]");
    items[1] = new_item(" ", "Step [n]");
    items[2] = new_item(" ", "Set width");
    items[3] = new_item(" ", "Set height");
    items[4] = new_item(" ", "Toggle board wrapping");
    items[5] = new_item(" ", "Set cell to alive");
    items[6] = new_item(" ", "Set cell to dead");
    items[7] = new_item(" ", "Clear board");
    items[8] = new_item(" ", "Set simulation speed");
    items[9] = new_item(" ", "Set rules");
    items[10] = new_item(" ", "Save board");
    items[11] = new_item(" ", "Load board");
    items[12] = new_item(" ", "Exit [q]");
    items[13] = NULL;

    menu = new_menu(items);

    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, derwin(menuWindow, 13, 25, 0, 0));

    keypad(menuWindow, TRUE);  // Enable input of special keys

    post_menu(menu);
    wrefresh(menuWindow);

    return menuWindow;
}

// TODO: delete
void updateMenuWindow(WINDOW* window)
{
    wrefresh(window);
}

int handleMenuInput(WINDOW* window, int ch)
{
    switch(ch)
    {
        case KEY_DOWN:
            if(selectedItemIndex++ < 12)
            {
                menu_driver(menu, REQ_DOWN_ITEM);
            }
            else
            {
                selectedItemIndex = 0;
                menu_driver(menu, REQ_FIRST_ITEM);
            }
            wrefresh(window);
            break;
        case KEY_UP:
            if(selectedItemIndex-- > 0)
            {
                menu_driver(menu, REQ_UP_ITEM);
            }
            else
            {
                selectedItemIndex = 12;
                menu_driver(menu, REQ_LAST_ITEM);
            }
            wrefresh(window);
            break;
        case ' ':
        // case KEY_ENTER:
            return selectedItemIndex;
    }

    return -1;
}
