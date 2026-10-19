#pragma once
#include "../game/gameStucts.h"

typedef enum policy{
    randomPolicy,
    learnedPolicy
} policy;

action random_policy(gameState S);

action learned_policy(gameState S);

action promptAgent(gameState S, policy P);
