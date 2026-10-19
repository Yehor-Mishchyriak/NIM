// std library includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// proj includes
#include "player/player.h"
#include "agent/agent.h"
#include "agent/rl.h"
#include "game/gameLoopControls.h"


int main(void){
    srand((unsigned int)time(NULL));

    printWelcomeMessage();
    printGameStateInitInstructions();
    gameState S = initGameState();
    if (!qtable_init(&qtable)){
        fprintf(stderr, "Could not allocate the Q-table.\n");
        return EXIT_FAILURE;
    }
    printf("Training against the random policy...\n");
    trainQTable(&qtable, S, 20000, 0.1, 0.2);
    printGameState(S);
    while (S.subState == PlayerGoes || S.subState == AgentGoes){
        action A;
        if (S.subState == PlayerGoes){
            A = promptPlayer(S);
        } else {
            A = promptAgent(S, learnedPolicy);
            printf("Agent removes %d stick(s) from pile %c.\n",
                   A.stickCountChoice, A.pileChoice);
        }
        S = advanceGameState(S, A);
        printGameState(S);
    }
    qtable_free(&qtable);
    return 0;
}
