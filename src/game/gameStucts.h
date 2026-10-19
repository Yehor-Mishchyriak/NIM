#pragma once

#ifndef MAX_PILE
#define MAX_PILE 25
#endif

typedef enum gameSubState
{
    PlayerGoes,
    AgentGoes,
    PlayerWon,
    AgentWon,
    Invalid
} gameSubState;

typedef struct action{
    char pileChoice;
    int stickCountChoice;
} action;

typedef struct gameState
{
    int pileA;
    int pileB;
    int pileC;
    gameSubState subState;
} gameState;
