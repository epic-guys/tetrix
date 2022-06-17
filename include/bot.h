#ifndef _BOT_H_
#define _BOT_H_

#include <tetrimino.h>
#include <gamefield.h>

typedef struct Strategy strategy_t;


strategy_t *strategy_create(int *field);
void strategy_destroy(strategy_t *strategy);
void strategy_update(strategy_t *strategy, tetrimino_t *tetrimino, int cur_pos);
void choose_strategy(gamefield_t *field, tetrimini_pool_t *pool);
int calculate_score(int *field);

#endif