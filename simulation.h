#ifndef SIMULATION_H
#define SIMULATION_H

#include "board.h"
#include "rules.h"

// Creates new board from old after 1 simulation step. Old board is destoryed
Board* stepSimulation(Board* board, Ruleset* ruleset);

#endif