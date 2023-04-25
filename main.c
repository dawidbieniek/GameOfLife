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
int changingBoard = 0;
Board* board;
Ruleset* ruleset;

int maxW;
int maxH;

int selectedX = 0;
int selectedY = 0;

void printSimulationInfo(int simStep, int gameState)
{
    mvprintw(0, 0, "Press q to exit");
    mvprintw(2, 0, "Simulation state: %s", gameState ? "playing" : "paused ");
    mvprintw(3, 18, "      ");
    refresh();
    mvprintw(3, 0, "Simulation step: %d", simStep);
    refresh();
}

void nextSimulationStep(Board* board, Ruleset* ruleset, int* simStep, int* gameState)
{
    stepSimulation(board, ruleset);
    (*simStep)++;

    pthread_mutex_lock(&refreshMutex);
    printSimulationInfo(*simStep, *gameState);
    updateBoardWindow(board, -1, -1);
    pthread_mutex_unlock(&refreshMutex);
}

void toggleSimulation()
{
    simState = !simState;
    printSimulationInfo(simStep, simState);

    if(simState)
    {
        lockMenuOptions(1);
        resumeGameThread();
    }
    else
    {
        lockMenuOptions(0);
        pauseGameThread();
    }

}

void setBoardW(int newW)
{
    boardW = newW;
    resizeBoard(board, boardW, boardH);
    resizeBoardWindow(boardW, boardH);
    updateBoardWindow(board, -1, -1);

    selectedX = 0;
    selectedY = 0;
}

void setBoardH(int newH)
{
    boardH = newH;
    resizeBoard(board, boardW, boardH);
    resizeBoardWindow(boardW, boardH);
    updateBoardWindow(board, -1, -1);

    selectedX = 0;
    selectedY = 0;
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
            nextSimulationStep(board, ruleset, &simStep, &simState);
            break;
        case 2:     /* Set width */
            showInputWindow("Set board width");
            input = handleInputWindowInput();
            intInput = atoi(input);

            if(intInput >= 1 && intInput <= maxW)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardW(intInput);
                pthread_mutex_unlock(&refreshMutex);
            }
            else
            {
                showError("Wrong input value");
            }

            break;
        case 3:     /* Set height */
            showInputWindow("Set board height");
            input = handleInputWindowInput();
            intInput = atoi(input);

            if(intInput >= 1 && intInput <= maxH)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardH(intInput);
                pthread_mutex_unlock(&refreshMutex);
            }
            else
            {
                showError("Wrong input value");
            }
            break;
        case 4:     /* Toggle wrapping */
            wrap = !wrap;
            setWrapping(wrap);
            break;
        case 5:     /* Set cell */
            changingBoard = 1;
            break;
        case 6:     /* Clear cell */
             changingBoard = 2;
            break;
        case 7:     /* Clear board */
            simStep = 0;
            if(simState)
            {
                toggleSimulation();
            }
            printSimulationInfo(simStep, simState);
            
            clearBoard(board);
            updateBoardWindow(board, -1, -1);
            break;
        case 8:     /* Set sim speed */
            showInputWindow("Set simulation speed");
            input = handleInputWindowInput();
            floatInput = atof(input);

            if(floatInput >= 0.5 && floatInput <= 10.0)
            {
                setSleepDuration(1000.0/floatInput);
            }
            else
            {
                showError( "Wrong input value");
            }
            break;
        case 9:     /* Set rules */
            showInputWindow("Set rules");
            input = handleInputWindowInput();
            
            if(!validateRules(input))
            {
                showError("Wrong input value");
            }
            else
            {
                parseRules(input, ruleset);
            }
            break;
        case 10:    /* Save board */
            showInputWindow("Save board (path)");
            input = handleInputWindowInput();

            if(!saveBoard(board, input))
            {
                showError("Cannot write to file");
            }
            break;
        case 11:    /* Load board */
            showInputWindow("Load board (path)");
            input = handleInputWindowInput();

            if(!loadBoard(board, input, maxW, maxH))
            {
                showError("Cannot read from file");
            }
            else
            {
                resizeBoardWindow(board->w, board->h);
                updateBoardWindow(board, -1, -1);
                simStep = 0;
                printSimulationInfo(simStep, simState);

                selectedX = 0;
                selectedY = 0;
                
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
    init_pair(3, 0, 7); /* Color board cell highlight */

#ifdef WIDE_MODE
    maxW = (COLS - 27) / 2;
#else
    maxW = COLS - 27;
#endif
    maxH = LINES - 5;

    printSimulationInfo(simStep, simState);

    boardWindow = createBoardWindow(board, 0, 4);
    menuWindow = createMenuWindow(COLS-25, 4);
    inputWindow = createInputWindow(COLS-25, 18);

    updateBoardWindow(board, -1, -1);
    updateMenuWindowUnsafe();

    refreshMutex = startGameThread(&nextSimulationStep, board, ruleset, &simStep, &simState);
    pauseGameThread();

    setMenuRefreshMutex(&refreshMutex);

    while(1)
    {
        ch = getch();

        if(changingBoard == 0)
        {
            if(ch == 'q') break;

            menuOpt = handleMenuInput(ch);
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
        }
        else
        {
            if(ch == 'q')
            {
                changingBoard = 0;
                updateBoardWindow(board, -1, -1);

                continue;;
            }

            switch(ch)
            {
                case KEY_LEFT:
                    selectedX--;
                    if(selectedX < 0)
                    {
                        selectedX = boardW - 1;
                    }
                    continue;;
                case KEY_RIGHT:
                    selectedX++;
                    if(selectedX >= boardW)
                    {
                        selectedX = 0;
                    }
                    continue;
                case KEY_UP:
                    selectedY--;
                    if(selectedY < 0)
                    {
                        selectedY = boardH - 1;;
                    }
                    continue;
                case KEY_DOWN:
                    selectedY++;
                    if(selectedY >= boardH)
                    {
                        selectedY = 0;
                    }
                    continue;
                case ' ':
                    setCell(selectedX, selectedY, changingBoard == 1 ? 1 : 0, board);
                    break;
            }

            updateBoardWindow(board, selectedX, selectedY);
        }

        /* Added some delay to slow down while loop when getch is set to not block thread */
        usleep(1000);
    }

    /* ## END ## */
    endwin();

    destroyGameThread();

    return 0;
}