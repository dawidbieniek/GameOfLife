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
    mvprintw(2, 0, "Simulation state: %s", gameState ? "playing" : "paused ");
    mvprintw(3, 0, "Simulation step: %d", simStep);
    refresh();
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
        resumeGameThread();
    }
    else
    {
        pauseGameThread();
    }
}

void oneStepSimulation()
{
    //TODO: Zamiast tego, powinien być blok w menu
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
    switch(menuOpt)
    {
        case 0:     // Start/Stop
            toggleSimulation();
            break;
        case 1:     // Step
            oneStepSimulation();
            break;
        case 2:     // Set width
            showInputWindow(inputWindow, "Set board width (1-50)");
            input = handleInputWindowInput(inputWindow);
            int newW = atoi(input);

            if(newW >= 1 && newW <= 50)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardW(newW);
                pthread_mutex_unlock(&refreshMutex);
            }

            break;
        case 3:     // Set height
            showInputWindow(inputWindow, "Set board height (1-50)");
            input = handleInputWindowInput(inputWindow);
            int newH = atoi(input);

            if(newH >= 1 && newH <= 50)
            {
                pthread_mutex_lock(&refreshMutex);
                setBoardH(newH);
                pthread_mutex_unlock(&refreshMutex);
            }
            break;
        case 4:     // Toggle wrapping
            wrap = !wrap;
            setWrapping(wrap);
            break;
        case 5:     // Set cell
            break;
        case 6:     // Clear cell
            break;
        case 7:     // Clear board
            clearBoard(board);
            updateBoardWindow(board, boardWindow);
            break;
        case 8:     // Set sim speed
            showInputWindow(inputWindow, "Set simulation speed (0.5-10)");
            input = handleInputWindowInput(inputWindow);
            float speed = atof(input);

            if(speed >= 0.5 && speed <= 10.0)
            {
                setSleepDuration(1000.0/speed);
            }
            break;
        case 9:     // Set rules
            showInputWindow(inputWindow, "Set rules (a->a/d->a)");
            input = handleInputWindowInput(inputWindow);
            
            parseRules(input, ruleset);
            break;
        case 10:    // Save board
            break;
        case 11:    // Load board
            break;
        case 12:    // Exit
            return 1;
    }
    return 0;
}

int main()
{
    int ch;
    int menuOpt = -1;

    board = createBoard(boardW, boardH);
    // Test purposes
    setCell(5,5,1,board);
    setCell(6,5,1,board);
    setCell(7,5,1,board);
    setCell(5,6,1,board);
    setCell(6,7,1,board);

    ruleset = createEmptyRuleset();
    parseRules("23/3", ruleset);   // Standard rules

    initscr();
    // ## START ##
    noecho();           // Disable printing of pressed characters in window
    cbreak();           // Disables line buffering
    keypad(stdscr, 1);  // Enable input of special keys
    curs_set(0);        // Hides cursor
    nodelay(stdscr, 1); // Disables lock for getch()

    printSimulationInfo(simStep, simState);

    boardWindow = createBoardWindow(board, 0, 4);
    menuWindow = createMenuWindow(60, 4);
    inputWindow = createInputWindow(60, 18);

    updateBoardWindow(board, boardWindow);
    updateMenuWindow(menuWindow);

    refreshMutex = startGameThread(&nextSimulationStep, board, ruleset, boardWindow, &simStep, &simState);
    pauseGameThread();

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
                toggleSimulation();
                break;
            case 'n':
                oneStepSimulation();
                break;
        }

        // Added some delay to slow down while loop when getch is set to not block thread
        usleep(1000);
    }

    // ## END ##
    endwin();

    destroyGameThread();

    return 0;
}