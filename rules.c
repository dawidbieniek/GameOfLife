#include "rules.h"

#include <string.h>
#include <stdlib.h>


Ruleset* initRuleset()
{
    Ruleset* rules = malloc(sizeof(Ruleset));

    for(int i = 0; i < 8; i++)
    {
        rules->aliveToAlive[i] = 0;
        rules->deadToAlive[i] = 0;
    }

    return rules;
}

int validateRules(char* rules)
{
    // Check for /
    int containsSlash = 0;
    for(int i = 0; i < strlen(rules); i++)
    {
        if(rules[i] == '/')
        {
            containsSlash = 1;
            break;
        }
    }
    if(!containsSlash) return 0;

    // Check if all are digits
    for(int i = 0; i < strlen(rules); i++)
    {
        if((rules[i] < '0' || rules[i] > '8') && rules[i] != '/')
        {
            return 0;
        }
    }

    return 1;
}

Ruleset* parseRules(char* rules)
{
    if(!validateRules(rules)) return NULL;

    Ruleset* ruleset = initRuleset();

    int beforeSlash = 1;
    for(int i = 0; i < strlen(rules); i++)
    {
        if(rules[i] == '/')
        {
            beforeSlash = 0;
            continue;
        }

        if(beforeSlash)
        {
            ruleset->aliveToAlive[rules[i] - '0'] = 1;
        }
        else
        {
            ruleset->deadToAlive[rules[i] - '0'] = 1;
        }
    }

    return ruleset;
}

char* rulesetToString(Ruleset* ruleset)
{
    int nextChar = 0;
    char* rules = calloc(18, sizeof(char));     // 8 alive-alive + 1 '/' + 8 dead-alive + 1 '\0'

    for(int i = 0; i < 8; i++)
    {
        if(ruleset->aliveToAlive[i])
        {
            rules[nextChar++] = i + '0';
        }
    }

    rules[nextChar++] = '/';

    for(int i = 0; i < 8; i++)
    {
        if(ruleset->deadToAlive[i])
        {
            rules[nextChar++] = i + '0';
        }
    }
    
    return rules;
}

int nextCellState(Ruleset* ruleset, int state, int neighbours)
{
    if(neighbours < 0 || neighbours > 8) return -1;

    if((state && ruleset->aliveToAlive[neighbours]) ||
        !state && ruleset->deadToAlive[neighbours])
    {
        return 1;
    }

    return 0;
}

void destroyRuleset(Ruleset* ruleset)
{
    free(ruleset);
    ruleset = NULL;
}