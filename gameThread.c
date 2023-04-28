#include "gameThread.h"

#include "rules.h"
#include "board.h"
#include <pthread.h>

#include <unistd.h>
#include <time.h>

Board* gameThread_board;
Ruleset* gameThread_rules;
int* gameThread_simStep;
int* gameThread_simState;

/* Thread */
pthread_t gameThread;
void (*loopedFunction)(Board*, Ruleset*, int*, int*);

/* Mutexes */
pthread_mutex_t suspendedMutex;
int isSuspended;
pthread_cond_t resumeCondition;

pthread_mutex_t gameThread_refreshMutex;

int sleepMs = 100;

void msleep()
{
    struct timespec ts;

    ts.tv_sec = sleepMs / 1000;
    ts.tv_nsec = (sleepMs % 1000) * 1000000;

    nanosleep(&ts, &ts);
}

void awaitResumed()
{
    pthread_mutex_lock(&suspendedMutex);
    while (isSuspended != 0) pthread_cond_wait(&resumeCondition, &suspendedMutex);
    pthread_mutex_unlock(&suspendedMutex);
}

void* gameThreadFunction()
{
    pthread_mutex_lock(&gameThread_refreshMutex);
    refresh();
    pthread_mutex_unlock(&gameThread_refreshMutex);
    while(1)
    {
        msleep();
        awaitResumed();
        loopedFunction(gameThread_board, gameThread_rules, gameThread_simStep, gameThread_simState);
    }
}

pthread_mutex_t startGameThread(void (*threadFunction)(Board*, Ruleset*, int*, int*), Board* board, Ruleset* ruleset, int* simStep, int* simState)
{ 
    pthread_mutex_init(&suspendedMutex, NULL);    
    pthread_mutex_init(&gameThread_refreshMutex, NULL);

    pthread_cond_init(&resumeCondition, NULL);

    loopedFunction = threadFunction;

    gameThread_board = board;
    gameThread_rules = ruleset;
    gameThread_simStep = simStep;
    gameThread_simState = simState;
    pthread_create(&gameThread, NULL, gameThreadFunction, NULL);

    return gameThread_refreshMutex;
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

void setSleepDuration(int mseconds)
{
    sleepMs = mseconds;
}

void destroyGameThread()
{
    pthread_cancel(gameThread);
    pthread_mutex_destroy(&suspendedMutex);
}