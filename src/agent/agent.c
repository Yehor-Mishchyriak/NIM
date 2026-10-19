// std library includes
#include <stdio.h>
#include <stdlib.h>
// proj includes
#include "agent.h"
#include "rl.h"

/* Requires nonnegative piles with at least one positive pile. */
action random_policy(gameState S){
    action A;
    int num_sticks;
    do {
        switch (rand()%3)
        {
        case 0:
            A.pileChoice = 'A';
            num_sticks = S.pileA;
            break;
        case 1:
            A.pileChoice = 'B';
            num_sticks = S.pileB;
            break;
        default:
            A.pileChoice = 'C';
            num_sticks = S.pileC;
            break;
        }
    } while (num_sticks == 0);
    A.stickCountChoice = 1 + rand()%num_sticks;
    return A;
}

action learned_policy(gameState S){
    return qtable_argmax(&qtable, S);
}

action promptAgent(gameState S, policy P){
    switch (P)
    {
    case randomPolicy:
        return random_policy(S);
    case learnedPolicy:
        return learned_policy(S);
    default:
        return random_policy(S);
    }
}
