#include <stdio.h>
#include "rules.h"
#include "board.h"
#include "simulation.h"

#include <ncurses.h>
#include <unistd.h>
// #include <pthread>

// void nextSimulationStep(Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep)
// {
//     Board* buffer;

//     buffer = stepSimulation(board, ruleset);
//     (*simStep)++;   // TODO: Może to w print
//     board = buffer;
//     buffer = NULL;

//     mvprintw(2, 0, "Simulation step: %d", *simStep);
//     refresh();
//     updateBoardWindow(board, boardWindow);
//     wrefresh(boardWindow);
// }

typedef struct
{
    int id;
} threadArgs;

int main()
{
    int boardW = 10;
    int boardH = 10;

    WINDOW* boardWindow;
    char ch;
    
    int simStep = 0;
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

    boardWindow = createBoardWindow(board, 0, 3);

    printw("Press q to exit");
    mvprintw(2, 0, "Simulation step: %d", simStep);
    refresh();

    updateBoardWindow(board, boardWindow);
    wrefresh(boardWindow);

    while((ch = getch()) != 'q')
    {
        // nextSimulationStep(board, ruleset, boardWindow, &simStep);
        
        Board* buffer;

        buffer = stepSimulation(board, ruleset);
        simStep++;
        board = buffer;
        buffer = NULL;

        mvprintw(2, 0, "Simulation step: %d", simStep);
        refresh();
        updateBoardWindow(board, boardWindow);
        wrefresh(boardWindow);
    }

    // ## END ##
    endwin();

    return 0;
}