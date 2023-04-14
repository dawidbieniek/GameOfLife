#ifndef SIMULATION_H
#define SIMULATION_H

#include "board.h"
#include "rules.h"

Board* stepSimulation(Board* board, Ruleset* ruleset);

#endif