#ifndef _BOT_H_
#define _BOT_H_

#include <tetrimino.h>
#include <gamefield.h>

typedef struct Strategy strategy_t;


strategy_t *strategy_create(int *field);
void strategy_destroy(strategy_t *strategy);
int get_strategy_cursor(strategy_t* s);
int get_strategy_tet_type(strategy_t* s);
int get_strategy_tet_rotation(strategy_t* s);
void strategy_update(strategy_t *strategy, tetrimino_t* tetrimino, int cur_pos,int rotation,int last_used_tetrimino);
strategy_t* choose_strategy(gamefield_t *field, tetrimini_pool_t *pool);
int calculate_score(int *field,int *old,int tet_type,int last_used_tet);
int compare_fields(int* new,int* old);

#endif