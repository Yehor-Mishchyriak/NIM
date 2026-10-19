#pragma once
#include <stdbool.h>
#include "gameStucts.h"

/* Requires a valid move on a nonempty board with an active turn.
   Returns the updated state; taking the last stick loses, otherwise turns switch. */
gameState advanceGameState(gameState S, action A);

/* Requires initialized S; checks piles in [0, MAX_PILE] and a non-Invalid subState.
   An empty board requires a winner. */
bool validateGameState(gameState S);

/* Requires initialized S and A; checks pile choice and removal bounds. */
bool validateAction(gameState S, action A);

/* Requires successful scanf conversions with representable input and eventual
   valid input. Returns piles in [0, MAX_PILE], at least one positive, and chosen turn. */
gameState initGameState(void);

/* Prints the welcome banner. */
void printWelcomeMessage(void);

/* Prints initialization instructions. */
void printGameStateInitInstructions(void);

/* Requires initialized S with a declared subState; prints its status. */
void printGameState(gameState S);
