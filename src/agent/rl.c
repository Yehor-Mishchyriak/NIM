#include <assert.h>
#include <stdlib.h>
#include "rl.h"
#include "agent.h"
#include "../game/gameLoopControls.h"

QTable qtable = {NULL, 0};

bool qtable_init(QTable *table){
    table->elements = calloc(QTableSize, sizeof(double));
    table->size = table->elements ? QTableSize : 0;
    return table->elements != NULL;
}

void qtable_free(QTable *table){
    free(table->elements);
    table->elements = NULL;
    table->size = 0;
}

size_t qtable_index(gameState S, action A){
    assert(S.pileA >= 0 && S.pileA <= MAX_PILE);
    assert(S.pileB >= 0 && S.pileB <= MAX_PILE);
    assert(S.pileC >= 0 && S.pileC <= MAX_PILE);
    assert(validateAction(S, A));
    const size_t width = MAX_PILE + 1;
    size_t state = ((size_t)S.pileA * width + S.pileB) * width + S.pileC;
    return (state * 3 + (A.pileChoice - 'A')) * width + A.stickCountChoice;
}

double qtable_get(const QTable *table, gameState S, action A){
    size_t index = qtable_index(S, A);
    assert(table->elements && index < table->size);
    return CTUPLE_GET(*table, index);
}

void qtable_set(QTable *table, gameState S, action A, double value){
    size_t index = qtable_index(S, A);
    assert(table->elements && index < table->size);
    CTUPLE_GET(*table, index) = value;
}

action qtable_argmax(const QTable *table, gameState S){
    int piles[] = {S.pileA, S.pileB, S.pileC};
    action best = {'A', 0};
    double bestValue = 0;
    int ties = 0;
    for (int pile = 0; pile < 3; ++pile){
        for (int count = 1; count <= piles[pile]; ++count){
            action candidate = {(char)('A' + pile), count};
            double value = qtable_get(table, S, candidate);
            if (ties == 0 || value > bestValue){
                best = candidate;
                bestValue = value;
                ties = 1;
            } else if (value == bestValue){
                ++ties;
                if (rand() % ties == 0){best = candidate;}
            }
        }
    }
    assert(ties > 0);
    return best;
}

void trainQTable(QTable *table, gameState bounds, size_t episodes,
                 double alpha, double epsilon){
    assert(validateGameState(bounds));
    assert(bounds.pileA > 0 || bounds.pileB > 0 || bounds.pileC > 0);
    assert(alpha > 0 && alpha <= 1 && epsilon >= 0 && epsilon <= 1);
    for (size_t episode = 0; episode < episodes; ++episode){
        gameState S;
        do {
            S = (gameState){rand() % (bounds.pileA + 1),
                            rand() % (bounds.pileB + 1),
                            rand() % (bounds.pileC + 1), AgentGoes};
        } while (S.pileA == 0 && S.pileB == 0 && S.pileC == 0);
        while (S.subState == AgentGoes){
            double sample = (double)rand() / ((double)RAND_MAX + 1.0);
            action A = sample < epsilon ? random_policy(S) : qtable_argmax(table, S);
            gameState next = advanceGameState(S, A);
            double target;
            if (next.subState == PlayerWon){
                target = -1;
            } else {
                next = advanceGameState(next, random_policy(next));
                if (next.subState == AgentWon){
                    target = 1;
                } else {
                    /* The next decision belongs to the learner again. */
                    action best = qtable_argmax(table, next);
                    target = qtable_get(table, next, best);
                }
            }
            double old = qtable_get(table, S, A);
            qtable_set(table, S, A, old + alpha * (target - old));
            S = next;
        }
    }
}
