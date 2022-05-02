#ifndef _BOT_H_
#define _BOT_H_

typedef struct config
{
    tetrimino_type_t tetrimino;
    int cursor;
    int rotation;
} config_t;


config_t get_best_config(gamefield_t *field, tetrimini_pool_t *pool);

#endif