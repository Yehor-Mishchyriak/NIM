#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/agent/agent.h"
#include "../src/agent/rl.h"
#include "../src/game/gameLoopControls.h"

int main(void){
    srand(42);
    assert(qtable_init(&qtable));
    assert(double_QTableSize_tuple_len(qtable) == QTableSize);
    unsigned char *seen = calloc(QTableSize, 1);
    assert(seen);

    for (int a = 0; a <= MAX_PILE; ++a){
        for (int b = 0; b <= MAX_PILE; ++b){
            for (int c = 0; c <= MAX_PILE; ++c){
                gameState S = {a, b, c, AgentGoes};
                int piles[] = {a, b, c};
                for (int pile = 0; pile < 3; ++pile){
                    for (int count = 1; count <= piles[pile]; ++count){
                        action A = {(char)('A' + pile), count};
                        size_t index = qtable_index(S, A);
                        assert(index < QTableSize && !seen[index]);
                        seen[index] = 1;
                        assert(qtable_get(&qtable, S, A) == 0);
                        qtable_set(&qtable, S, A, -(double)(index + 1));
                        S.subState = PlayerGoes;
                        assert(qtable_index(S, A) == index);
                        assert(qtable_get(&qtable, S, A) == -(double)(index + 1));
                        S.subState = AgentGoes;
                    }
                }
                if (a + b + c == 0){continue;}
                action best = qtable_argmax(&qtable, S);
                assert(validateAction(S, best));
                for (int pile = 0; pile < 3; ++pile){
                    for (int count = 1; count <= piles[pile]; ++count){
                        action A = {(char)('A' + pile), count};
                        assert(qtable_get(&qtable, S, best) >= qtable_get(&qtable, S, A));
                    }
                }
                for (int i = 0; i < 10; ++i){
                    assert(validateAction(S, random_policy(S)));
                }
            }
        }
    }
    free(seen);
    qtable_free(&qtable);
    assert(qtable.elements == NULL && qtable.size == 0);
    assert(qtable_init(&qtable));

    /* Removing one of two sticks forces the opponent to take the last. */
    trainQTable(&qtable, (gameState){2, 0, 0, AgentGoes}, 10000, 0.1, 0.2);
    for (int n = 1; n <= 2; ++n){
        gameState S = {n, 0, 0, AgentGoes};
        action last = {'A', n};
        assert(fabs(qtable_get(&qtable, S, last) + 1) < 0.001);
    }
    gameState S = {2, 0, 0, AgentGoes};
    action winning = {'A', 1};
    assert(fabs(qtable_get(&qtable, S, winning) - 1) < 0.001);
    assert(learned_policy(S).stickCountChoice == 1);

    /* Exercise nonterminal updates and full games on the bounded state space. */
    trainQTable(&qtable, (gameState){MAX_PILE, MAX_PILE, MAX_PILE, AgentGoes},
                10000, 0.1, 0.2);
    for (int game = 0; game < 100; ++game){
        S = (gameState){MAX_PILE, MAX_PILE, MAX_PILE, game % 2 ? PlayerGoes : AgentGoes};
        int moves = 0;
        while (S.subState == AgentGoes || S.subState == PlayerGoes){
            action A = S.subState == AgentGoes ? learned_policy(S) : random_policy(S);
            assert(validateAction(S, A));
            S = advanceGameState(S, A);
            assert(validateGameState(S));
            assert(++moves <= 3 * MAX_PILE);
        }
        assert(S.subState == PlayerWon || S.subState == AgentWon);
    }
    for (size_t i = 0; i < qtable.size; ++i){
        assert(isfinite(CTUPLE_GET(qtable, i)));
        assert(CTUPLE_GET(qtable, i) >= -1 && CTUPLE_GET(qtable, i) <= 1);
    }
    qtable_free(&qtable);
    puts("Q-table and training checks passed.");
    return 0;
}
