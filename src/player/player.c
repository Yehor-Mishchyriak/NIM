// std library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// proj includes
#include "player.h"
#include "../game/gameLoopControls.h"

action promptPlayer(gameState S){
    action A;
    char whichPile;
    do {
        printf("Which pile to draw sticks from? (A, B, C)\n");
        scanf(" %c", &(whichPile));
        if (whichPile == 'A'){A.pileChoice = 'A';}
        else if (whichPile == 'B'){A.pileChoice = 'B';}
        else if (whichPile == 'C'){A.pileChoice = 'C';}
        else {
            printf("Invalid action. Choose pile A, B, or C.\n");
            int ch;
            while ((ch=getchar()) != '\n' && ch != EOF){}
            continue;
        }
        printf("How many sticks to draw?\n");
        scanf(" %d", &(A.stickCountChoice));
        if (validateAction(S, A)) {return A;}
        printf("Invalid action. Remove at least one stick and no more than the chosen pile contains.\n");
    } while (true);
    return A;
}
