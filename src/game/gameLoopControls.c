#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gameLoopControls.h"

/* Requires successful scanf conversions with representable input.
   Returns piles in [0, MAX_PILE], at least one positive, and chosen first turn. */
gameState initGameState(void){
    gameState S;
    char goesFirst;
    do {
        printf("# sticks in pile A (0-%d) = \n", MAX_PILE);
        scanf(" %d", &(S.pileA));
        printf("# sticks in pile B (0-%d) = \n", MAX_PILE);
        scanf(" %d", &(S.pileB));
        printf("# sticks in pile C (0-%d) = \n", MAX_PILE);
        scanf(" %d", &(S.pileC));
        printf("# goes first ('P' for player, 'A' for agent) = \n");
        scanf(" %c", &(goesFirst));
        if (goesFirst=='P'){S.subState=PlayerGoes;}
        else if(goesFirst=='A'){S.subState=AgentGoes;}
        else{S.subState=Invalid;}
        if (validateGameState(S)){return S;}
        else {printf("Invalid game state. Please follow the instructions to initialize the game.\n");}
        int ch;
        while ((ch=getchar()) != '\n' && ch != EOF){}
    } while (true);

    return S;
}

/* Prints the welcome banner. */
void printWelcomeMessage(void){
    printf("==========================\nWELCOME TO THE GAME OF NIM!\n==========================\n");
}

/* Prints initialization instructions. */
void printGameStateInitInstructions(void){
    printf("Specify integers from 0 to %d for piles A, B, and C, with at least one positive.\n", MAX_PILE);
    printf("Then, specify who goes first: player, or the agent.\n");
}

/* Requires initialized S with a declared subState; prints its status. */
void printGameState(gameState S){
    if (S.subState == PlayerWon){
        printf("The player has won!\n");
        return;
    }
    if (S.subState == AgentWon){
        printf("The agent has won!\n");
        return;
    }
    if (S.subState == Invalid){
        printf("ERROR: invalid game state.\n");
        return;
    }
    printf("# sticks in pile A is %d.\n", S.pileA);
    printf("# sticks in pile B is %d.\n", S.pileB);
    printf("# sticks in pile C is %d.\n", S.pileC);
    printf("It is %s's turn.\n", S.subState == PlayerGoes ? "player" : "agent");
}

/* Requires initialized S; checks piles in [0, MAX_PILE] and a non-Invalid subState.
   An empty board requires a winner. */
bool validateGameState(gameState S){
    bool validPileA = S.pileA >= 0 && S.pileA <= MAX_PILE;
    bool validPileB = S.pileB >= 0 && S.pileB <= MAX_PILE;
    bool validPileC = S.pileC >= 0 && S.pileC <= MAX_PILE;
    if (S.pileA == 0 && S.pileB == 0 && S.pileC == 0){
        if (S.subState != PlayerWon && S.subState != AgentWon){return false;}
    }
    bool validSubState = S.subState != Invalid;
    return validPileA && validPileB && validPileC && validSubState;
}

/* Requires initialized S and A; checks pile choice and removal bounds. */
bool validateAction(gameState S, action A){
    if (A.pileChoice == 'A'){
        return (A.stickCountChoice > 0 && A.stickCountChoice <= S.pileA);
    }
    if (A.pileChoice == 'B'){
        return (A.stickCountChoice > 0 && A.stickCountChoice <= S.pileB);
    }
    if (A.pileChoice == 'C'){
        return (A.stickCountChoice > 0 && A.stickCountChoice <= S.pileC);
    }
    else {
        return false;
    }
}

/* Requires a valid move on a nonempty board with an active turn.
   Removes sticks and switches turns; taking the last stick loses. */
gameState advanceGameState(gameState S, action A){
    gameState newS = S;
    if (A.pileChoice == 'A'){
        newS.pileA -= A.stickCountChoice;
    }
    if (A.pileChoice == 'B'){
        newS.pileB -= A.stickCountChoice;
    }
    if (A.pileChoice == 'C'){
        newS.pileC -= A.stickCountChoice;
    }
    if (newS.pileA == 0 && newS.pileB == 0 && newS.pileC == 0){
        newS.subState = S.subState == PlayerGoes ? AgentWon : PlayerWon;
    } else if (S.subState == PlayerGoes){
        newS.subState = AgentGoes;
    } else {
        newS.subState = PlayerGoes;
    }
    return newS;
}
