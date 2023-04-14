#include <stdio.h>
#include "rules.h"
#include "board.h"
#include "simulation.h"


int main()
{
    Ruleset* rules = parseRules("23/3");

    printf("%s\n", rulesetToString(rules));

    printf("alive, 1: %d\n", nextCellState(rules, 1, 1));
    printf("alive, 2: %d\n", nextCellState(rules, 1, 2));
    printf("alive, 3: %d\n", nextCellState(rules, 1, 3));
    printf("alive, 4: %d\n", nextCellState(rules, 1, 4));

    printf("dead, 1: %d\n", nextCellState(rules, 0, 1));
    printf("dead, 2: %d\n", nextCellState(rules, 0, 2));
    printf("dead, 3: %d\n", nextCellState(rules, 0, 3));
    printf("dead, 4: %d\n", nextCellState(rules, 0, 4));

    Board* board = createBoard(10, 10);
    // setCell(4, 3, 1, board);
    // setCell(4, 4, 1, board);
    // setCell(4, 5, 1, board);
    // setCell(1, 0, 1, board);
    // setCell(1, 1, 1, board);
    // setCell(1, 2, 1, board);
    setCell(5,5,1,board);
    setCell(6,5,1,board);
    setCell(7,5,1,board);
    setCell(5,6,1,board);
    setCell(6,7,1,board);
    writeBoard(board);
    Board* nboard;

    char c;
    do
    {
        nboard = stepSimulation(board, rules);
        writeBoard(nboard);
        board = nboard;
        nboard = NULL;
        c = getchar();
    } while(c != 'q');

    return 0;
}