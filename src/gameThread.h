#ifndef GAMETHREAD_H
#define GAMETHREAD_H

#include "rules.h"
#include "board.h"
#include <pthread.h>

/* Creates game thread and starts it */
pthread_mutex_t startGameThread(void (*threadFunction)(Board*, Ruleset*, int*, int*), Board* board, Ruleset* ruleset, int* simStep, int* simState);

/* Unpauses game thread */
void resumeGameThread();

/* Pauses game thread */
void pauseGameThread();

/* Sets the duration of sleep after each loop */
void setSleepDuration(int mseconds);

/* Destroys game thread and mutexes */
void destroyGameThread();
#endif
