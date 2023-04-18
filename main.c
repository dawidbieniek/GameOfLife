#include <stdio.h>
#include "rules.h"
#include "board.h"
#include "simulation.h"
#include "gameThread.h"
#include "gameMenu.h"
#include "inputWindow.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#include <pthread.h>

pthread_mutex_t refreshMutex;

int boardW = 20;
int boardH = 20;
int wrap = 0;

WINDOW* boardWindow;
WINDOW* menuWindow;
WINDOW* inputWindow;

int simStep = 0;
int simState = 0;
Board* board;
Ruleset* ruleset;

void printSimulationInfo(int simStep, int gameState)
{
    mvprintw(0, 0, "Press q to exit");
    move(2,0);
    clrtoeol();
    refresh();
    mvprintw(2, 0, "Simulation state: %s", gameState ? "playing" : "paused ");
    mvprintw(3, 0, "Simulation step: %d", simStep);
    refresh();
    wrefresh(stdscr);
}

void nextSimulationStep(Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep, int* gameState)
{
    stepSimulation(board, ruleset);
    (*simStep)++;

    pthread_mutex_lock(&refreshMutex);
    printSimulationInfo(*simStep, *gameState);
    updateBoardWindow(board, boardWindow);
    pthread_mutex_unlock(&refreshMutex);
}

void toggleSimulation()
{
    simState = !simState;
    printSimulationInfo(simStep, simState);

    if(simState)
    {
        lockMenuOptions(menuWindow, 1);
        resumeGameThread();
    }
    else
    {
        lockMenuOptions(menuWindow, 0);
        pauseGameThread();
    }

}

void oneStepSimulation()
{
    /* TODO: Zamiast tego, powinien być blok w menu */
    if(!simState)
    {
        nextSimulationStep(board, ruleset, boardWindow, &simStep, &simState);
    }
}

void setBoardW(int newW)
{
    boardW = newW;
    resizeBoard(board, boardW, boardH);
    resizeBoardWindow(boardWindow, boardW, boardH);
    updateBoardWindow(board, boardWindow);
}

void setBoardH(int newH)
{
    boardH = newH;
    resizeBoard(board, boardW, boardH);
    resizeBoardWindow(boardWindow, boardW, boardH);
    updateBoardWindow(board, boardWindow);
}

int handleMenuPress(int menuOpt)
{
    char* input;
    int intInput;
    float floatInput;

    if(!isMenuOptionAviable(menuOpt, simState))
    {
        return 0;
    }

    switch(menuOpt)
    {
        case 0:     /* Start/Stop */
            toggleSimulation();
            break;
        case 1:     /* Step */
            oneStepSimulation();
            break;
        case 2:     /* Set width */
            showInputWindow(inputWindow, "Set board width (1-50)");
            input = handleInputWindowInput(inputWindow);
            intInput = atoi(input);

            if(intInput >= 1 && intInput <= 50)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardW(intInput);
                pthread_mutex_unlock(&refreshMutex);
            }
            else
            {
                showError(inputWindow, "Wrong input value");
            }

            break;
        case 3:     /* Set height */
            showInputWindow(inputWindow, "Set board height (1-50)");
            input = handleInputWindowInput(inputWindow);
            intInput = atoi(input);

            if(intInput >= 1 && intInput <= 50)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardH(intInput);
                pthread_mutex_unlock(&refreshMutex);
            }
            else
            {
                showError(inputWindow, "Wrong input value");
            }
            break;
        case 4:     /* Toggle wrapping */
            wrap = !wrap;
            setWrapping(wrap);
            break;
        case 5:     /* Set cell */
            break;
        case 6:     /* Clear cell */
            break;
        case 7:     /* Clear board */
            if(simState)
            {
                toggleSimulation();
            }
            simStep = 0;
            clearBoard(board);
            updateBoardWindow(board, boardWindow);
            break;
        case 8:     /* Set sim speed */
            showInputWindow(inputWindow, "Set simulation speed (0.5-10)");
            input = handleInputWindowInput(inputWindow);
            floatInput = atof(input);

            if(floatInput >= 0.5 && floatInput <= 10.0)
            {
                setSleepDuration(1000.0/floatInput);
            }
            else
            {
                showError(inputWindow, "Wrong input value");
            }
            break;
        case 9:     /* Set rules */
            showInputWindow(inputWindow, "Set rules (a->a/d->a)");
            input = handleInputWindowInput(inputWindow);
            
            if(!validateRules(input))
            {
                showError(inputWindow, "Wrong input value");
            }
            else
            {
                parseRules(input, ruleset);
            }
            break;
        case 10:    /* Save board */
            showInputWindow(inputWindow, "Save board (path)");
            input = handleInputWindowInput(inputWindow);

            if(!saveBoard(board, input))
            {
                showError(inputWindow, "Cannot write to file");
            }
            break;
        case 11:    /* Load board */
            showInputWindow(inputWindow, "Load board (path)");
            input = handleInputWindowInput(inputWindow);

            if(!loadBoard(board, input))
            {
                showError(inputWindow, "Cannot read from file");
            }
            else
            {
                resizeBoardWindow(boardWindow, board->w, board->h);
                updateBoardWindow(board, boardWindow);
                simStep = 0;
                printSimulationInfo(simStep, simState);
                
            }
            break;
        case 12:    /* Exit */
            return 1;
    }
    return 0;
}

int main()
{
    int ch;
    int menuOpt;
    
    menuOpt = -1;

    board = createBoard(boardW, boardH);
    /* Test purposes */
    setCell(5,5,1,board);
    setCell(6,5,1,board);
    setCell(7,5,1,board);
    setCell(5,6,1,board);
    setCell(6,7,1,board);

    ruleset = createEmptyRuleset();
    parseRules("23/3", ruleset);   /* Standard rules */

    initscr();
    /* ## START ## */
    start_color();      /* TODO: check if can use color */
    noecho();           /* Disable printing of pressed characters in window */
    cbreak();           /* Disables line buffering */
    keypad(stdscr, 1);  /* Enable input of special keys */
    curs_set(0);        /* Hides cursor */
    nodelay(stdscr, 1); /* Disables lock for getch() */
    
    nodelay(inputWindow, 0); /* Enables lock for getch() for input window */

    init_pair(1, 8, 0); /* Color for unselectable menu items */
    init_pair(2, 1, 0); /* Color for error message */

    printSimulationInfo(simStep, simState);

    boardWindow = createBoardWindow(board, 0, 4);
    menuWindow = createMenuWindow(60, 4);
    inputWindow = createInputWindow(60, 18);

    updateBoardWindow(board, boardWindow);
    updateMenuWindowUnsafe(menuWindow);

    refreshMutex = startGameThread(&nextSimulationStep, board, ruleset, boardWindow, &simStep, &simState);
    pauseGameThread();

    setMenuRefreshMutex(&refreshMutex);

    while((ch = getch()) != 'q')
    {
        menuOpt = handleMenuInput(menuWindow, ch);
        if(menuOpt > -1)
        {
            if(handleMenuPress(menuOpt))
            {
                break;
            }
        }

        switch(ch)
        {
            case 'p':
                handleMenuPress(0);
                break;
            case 'n':
                handleMenuPress(1);
                break;
        }

        /* Added some delay to slow down while loop when getch is set to not block thread */
        usleep(1000);
    }

    /* ## END ## */
    endwin();

    destroyGameThread();

    return 0;
}