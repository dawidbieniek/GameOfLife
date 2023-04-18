#include "gameThread.h"

#include "rules.h"
#include "board.h"
#include <pthread.h>

#include <unistd.h>

Board* boardRef;
Ruleset* rulesetRef;
WINDOW* boardWindowRef;
int* simStepRef;
int* simStateRef;

/* Thread */
pthread_t gameThread;
void (*loopedFunction)(Board*, Ruleset*, WINDOW*, int*, int*);

/* Mutexes*/
pthread_mutex_t suspendedMutex;
int isSuspended;
pthread_cond_t resumeCondition;
pthread_mutex_t refreshMutexRef;

int sleepUs = 100000;

void* gameThreadFunction()
{
    refresh();
    while(1)
    {
        usleep(sleepUs);
        awaitResumed();
        loopedFunction(boardRef, rulesetRef, boardWindowRef, simStepRef, simStateRef);
    }
}

pthread_mutex_t startGameThread(void (*threadFunction)(Board*, Ruleset*, WINDOW*, int*, int*), Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep, int* simState)
{ 
    pthread_mutex_init(&suspendedMutex, NULL);    
    pthread_mutex_init(&refreshMutexRef, NULL);

    loopedFunction = threadFunction;

    boardRef = board;
    rulesetRef = ruleset;
    boardWindowRef = boardWindow;
    simStepRef = simStep;
    simStateRef = simState;
    pthread_create(&gameThread, NULL, gameThreadFunction, NULL);

    return refreshMutexRef;
}

void resumeGameThread()
{
    pthread_mutex_lock(&suspendedMutex);
    isSuspended = 0;
    pthread_cond_broadcast(&resumeCondition);
    pthread_mutex_unlock(&suspendedMutex);
}

void pauseGameThread()
{
    pthread_mutex_lock(&suspendedMutex);
    isSuspended = 1;
    pthread_mutex_unlock(&suspendedMutex);
}

void awaitResumed()
{
    pthread_mutex_lock(&suspendedMutex);
    while (isSuspended != 0) pthread_cond_wait(&resumeCondition, &suspendedMutex);
    pthread_mutex_unlock(&suspendedMutex);
}

void setSleepDuration(int mseconds)
{
    sleepUs = mseconds * 1000;
}

void destroyGameThread()
{
    pthread_cancel(gameThread);
    pthread_mutex_destroy(&suspendedMutex);
}