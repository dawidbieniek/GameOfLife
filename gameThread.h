#ifndef GAMETHREAD_H
#define GAMETHREAD_H

#include "rules.h"
#include "board.h"

// TMP
// struct threadArgs
// {
//     Board* board;
//     Ruleset* ruleset;
//     WINDOW* boardWindow;
//     int* simStep;
// };

// Creates game thread and starts it
void startGameThread(void (*threadFunction)(Board*, Ruleset*, WINDOW*, int*), Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep);

// Unpauses game thread
void resumeGameThread();

// Pauses game thread
void pauseGameThread();

// Awaits resume of game thread. If thread is not paused, does nothing
void awaitResumed();

void destroyGameThread();
#endif
