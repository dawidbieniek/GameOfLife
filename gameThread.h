#ifndef GAMETHREAD_H
#define GAMETHREAD_H

#include "rules.h"
#include "board.h"
#include <pthread.h>

/* Creates game thread and starts it */
pthread_mutex_t startGameThread(void (*threadFunction)(Board*, Ruleset*, WINDOW*, int*, int*), Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep, int* simState);

/* Unpauses game thread */
void resumeGameThread();

/* Pauses game thread */
void pauseGameThread();

/* Awaits resume of game thread. If thread is not paused, does nothing */
void awaitResumed();

/* Sets the duration of sleep after each loop */
void setSleepDuration(int mseconds);

/* Destroys game thread and mutexes */
void destroyGameThread();
#endif
