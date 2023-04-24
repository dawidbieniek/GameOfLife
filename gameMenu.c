#include "gameMenu.h"

#include <ncurses.h>
#include <pthread.h>

#include <stdlib.h>
#include <menu.h>

const int lockableOptions[] = 
{
    1,      /* Step */
    2,      /* Set width */
    3,      /* Set height */
    5,      /* Set cell to alive */
    6,      /* Set cell to dead */
    9,      /* Set rules */
    10,     /* Save board */
    11      /* Load board */
};

MENU* menu;
ITEM** items;
int selectedItemIndex = 0;

pthread_mutex_t* menuRefreshMutex;

WINDOW* createMenuWindow(int x, int y)
{
    WINDOW* menuWindow= newwin(13, 30, y, x);

    items = calloc(14, sizeof(ITEM*));
    items[0] = new_item(" ", "Toggle simulation [p]");
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

    set_menu_grey(menu, COLOR_PAIR(1));

    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, derwin(menuWindow, 13, 25, 0, 0));

    keypad(menuWindow, TRUE);  /* Enable input of special keys */

    post_menu(menu);
    wrefresh(menuWindow);

    return menuWindow;
}

void setMenuRefreshMutex(pthread_mutex_t* refreshMutex)
{
    menuRefreshMutex = refreshMutex;
}

void updateMenuWindow(WINDOW* window)
{
    pthread_mutex_lock(menuRefreshMutex);
    wrefresh(window);
    pthread_mutex_unlock(menuRefreshMutex);
}

void updateMenuWindowUnsafe(WINDOW* window)
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
            updateMenuWindow(window);
            break;
        case ' ':
        /* case KEY_ENTER: */
            return selectedItemIndex;
    }

    return -1;
}

void lockMenuOptions(WINDOW* window, int lock)
{
    int i;

    if(lock)
    {
        for(i = 0; i < 8; i++)
        {
            item_opts_off(items[lockableOptions[i]], O_SELECTABLE);
        }
    }
    else
    {
        for(i = 0; i < 8; i++)
        {
            item_opts_on(items[lockableOptions[i]], O_SELECTABLE);
        }
    }
    updateMenuWindow(window);
}

int isMenuOptionAviable(int menuOpt, int simState)
{
    int i;

    if(simState)
    {
        for(i = 0; i < 8; i++)
        {
            if(lockableOptions[i] == menuOpt)
            {
                return 0;
            }
        }
    }

    return 1;
}
