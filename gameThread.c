#include "gameThread.h"

#include "rules.h"
#include "board.h"

#include <pthread.h>
#include <unistd.h>

struct threadArgs
{
    Board* board;
    Ruleset* ruleset;
    WINDOW* boardWindow;
    int* simStep;
    int* simState;
};

// Thread
pthread_t gameThread;
struct threadArgs gameThreadArgs;
void (*loopedFunction)(Board*, Ruleset*, WINDOW*, int*, int*);

// Mutexes
pthread_mutex_t suspendedMutex;
int isSuspended;
pthread_cond_t resumeCondition;

int sleepUs = 1000;

void* gameThreadFunction(void* functionArgs)
{
    struct threadArgs* args = (struct threadArgs*)functionArgs;
    Board* board = args->board;
    Ruleset* ruleset = args->ruleset;
    WINDOW* boardWindow = args->boardWindow;
    int* simStep = args->simStep;
    int* simState = args->simState;

    refresh();
    while(1)
    {
        usleep(sleepUs);
        awaitResumed();
        loopedFunction(board, ruleset, boardWindow, simStep, simState);
    }
}

void startGameThread(void (*threadFunction)(Board*, Ruleset*, WINDOW*, int*, int*), Board* board, Ruleset* ruleset, WINDOW* boardWindow, int* simStep, int* simState)
{ 
    pthread_mutex_init(&suspendedMutex, NULL);

    loopedFunction = threadFunction;

    gameThreadArgs.board = board;
    gameThreadArgs.ruleset = ruleset;
    gameThreadArgs.boardWindow = boardWindow;
    gameThreadArgs.simStep = simStep;
    gameThreadArgs.simState = simState;
    pthread_create(&gameThread, NULL, gameThreadFunction, &gameThreadArgs);
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