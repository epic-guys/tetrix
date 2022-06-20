#include "ncurses.h"

#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

/**
 * @brief Codifica di ogni tetramino.
 * 
 * T_I:
 * ████████
 * T_J:
 *   ██
 *   ██
 * ████
 * T_L:
 * ██
 * ██
 * ████
 * T_S:
 *   ████
 * ████
 * T_O:
 * ████
 * ████
 * T_Z:
 * ████
 *   ████
 * T_T:
 * ██████
 *   ██
 */
typedef enum tetrimino_type
{
    T_I = 0,
    T_J = 1,
    T_L = 2,
    T_S = 3,
    T_O = 4,
    T_Z = 5,
    T_T = 6,
} tetrimino_type_t;

typedef struct tetrimino_config
{
    tetrimino_type_t type;
    int rotation;
    int cursor;
} tetrimino_config_t;


typedef struct Tetrimino tetrimino_t;

tetrimino_t *get_tetrimino(int type);

void free_tetrimino(tetrimino_t *t);

void print_tetrimino(WINDOW *w,tetrimino_t *t,int y,int x);

typedef struct TetriminiPool tetrimini_pool_t;

tetrimini_pool_t *initialize_pool(int x, int y);

void free_pool(tetrimini_pool_t* p);

void print_menu_style(int i, tetrimini_pool_t *pool);

int select_tetrimino(tetrimini_pool_t *pool);

int safe_rotate_tetrimino(tetrimino_t *t, int cur_pos, int dir);

void refresh_pool(tetrimini_pool_t *p);

void remove_tetrimino_from_pool(int i, tetrimini_pool_t *pool);

void add_tetrimino_from_pool(int i, tetrimini_pool_t *p);

int no_tetriminos_left(tetrimini_pool_t *pool);

WINDOW *get_pool_win(tetrimini_pool_t *t);

int get_remaining_tetriminos(tetrimini_pool_t *pool, tetrimino_type_t type);

int get_tet_rows(tetrimino_t *t);

int get_tet_cols(tetrimino_t *t);

int *get_tet_values(tetrimino_t *t);

int get_tet_type(tetrimino_t *t);

int get_tet_color(tetrimino_t *t);

int is_safe_to_place_tet(tetrimino_t *t);

int are_tet_equal(tetrimino_t* f,tetrimino_t* s);

#endif
