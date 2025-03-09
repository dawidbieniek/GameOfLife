#ifndef RULES_H
#define RULES_H

typedef struct
{
    int aliveToAlive[8]; 
    int deadToAlive[8];
} Ruleset;

/* Creates new ruleset and fills it with 0s */
Ruleset* createEmptyRuleset();

/* Checks if string is in correct format */
int validateRules(char* rules);

/* Parses rules and sets ruleset according to input string */
void parseRules(char* rules, Ruleset* ruleset);

/* Creates string from ruleset */
char* rulesetToString(Ruleset* ruleset);

/* Returns state of cell in next frame based on number of neighbours. -1 if error */
int nextCellState(Ruleset* ruleset, int state, int neighbours);

/* Frees memory of ruleset */
void destroyRuleset(Ruleset* ruleset);
#endif