#ifndef SIMULATION_H
#define SIMULATION_H

#include "board.h"
#include "rules.h"

/* Updates values of cells in board to their next state in simulation */
void stepSimulation(Board* board, Ruleset* ruleset);

/* Sets wrapping of board to given value */
void setWrapping(int value);
#endif