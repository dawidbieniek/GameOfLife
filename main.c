#include <stdio.h>
#include "rules.h"
#include "board.h"
#include "simulation.h"
#include "gameThread.h"

#include <ncurses.h>


void printSimulationInfo(int simStep, int gameState)
{
    mvprintw(0, 0, "Press q to exit");
    mvprintw(2, 0, "Simulation state: %s", gameState ? "playing" : "paused");
    mvprintw(3, 0, "Simulation step: %d", simStep);
    refresh();
}

void nextSimulationStep(Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep, int* gameState)
{    
    if(!(*gameState))
    {
        (*gameState) = 1;
    }

    stepSimulation(board, ruleset);
    (*simStep)++;

    printSimulationInfo(*simStep, *gameState);
    updateBoardWindow(board, boardWindow);
    wrefresh(boardWindow);
}


int main()
{
    int boardW = 50;
    int boardH = 50;

    WINDOW* boardWindow;
    char ch;
    
    int simStep = 0;
    int simState = 0;
    Board* board;
    Ruleset* ruleset;

    board = createBoard(boardW, boardH);
    // Test purposes
    setCell(5,5,1,board);
    setCell(6,5,1,board);
    setCell(7,5,1,board);
    setCell(5,6,1,board);
    setCell(6,7,1,board);

    ruleset = parseRules("23/3");   // Standard rules

    initscr();
    // ## START ##
    noecho();           // Disable printing of pressed characters in window
    cbreak();           // Allow window to get keyboard input
    keypad(stdscr, 1);  // Enable input of special keys
    curs_set(0);        // Hides cursor
    // nodelay(stdscr, 1); // Disables lock for getch()

    boardWindow = createBoardWindow(board, 0, 4);

    printSimulationInfo(simStep, simState);

    updateBoardWindow(board, boardWindow);
    wrefresh(boardWindow);

    startGameThread(&nextSimulationStep, board, ruleset, boardWindow, &simStep, &simState);
    pauseGameThread();

    while((ch = getch()) != 'q')
    {
        refresh();
    
        switch(ch)
        {
            case 'a':
                resumeGameThread();
                break;
            case 's':
                pauseGameThread();
                break;
            case 'd':
                nextSimulationStep(board, ruleset, boardWindow, &simStep, &simState);
                break;
        }
    }

    // ## END ##
    endwin();

    destroyGameThread();

    return 0;
}