#pragma once
#include <stdbool.h>
#include "../game/gameStucts.h"
#include "../util/ctuple.h"

#define QTableSize ((size_t)(MAX_PILE+1)*(MAX_PILE+1)*(MAX_PILE+1)*3*(MAX_PILE+1))
DEFINE_TUPLE_TYPE(double, QTableSize)
DEFINE_TUPLE_SIZE(double, QTableSize)
typedef double_QTableSize_tuple QTable;

extern QTable qtable;

/* Initialize once before use; free when finished. Entries start at zero. */
bool qtable_init(QTable *table);
void qtable_free(QTable *table);

/* Requires bounded piles and a legal action; ignores whose turn it is. */
size_t qtable_index(gameState S, action A);
double qtable_get(const QTable *table, gameState S, action A);
void qtable_set(QTable *table, gameState S, action A, double value);

/* Requires a nonempty bounded board; maximizes over legal actions, random ties. */
action qtable_argmax(const QTable *table, gameState S);

/* Train against random within bounds' pile sizes; gamma = 1. */
void trainQTable(QTable *table, gameState bounds, size_t episodes,
                 double alpha, double epsilon);
