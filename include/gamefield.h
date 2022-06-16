#include "tetrimino.h"

#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

typedef struct GameField gamefield_t;

gamefield_t *initialize_gamefield(int y, int x);

void free_gamefield(gamefield_t* g);

void clear_top(gamefield_t *g);

void refresh_selector(gamefield_t *g, tetrimino_t *t, int cur_pos);

void refresh_gamefield(gamefield_t *g);

int manage_drop(gamefield_t *gameField, tetrimino_t *t);

int get_first_free_row(int *f, tetrimino_t * t, int cur_pos);

int add_tetrimino_to_gamefield(gamefield_t *g,tetrimino_t *t,int cur_pos);

int add_tetrimino_to_field(int *f, tetrimino_t * t, int cur_pos);

int is_gamefield_top_occupied(gamefield_t *g);

int is_field_top_occupied(int *f);

int* get_gamefield(gamefield_t *g);

void set_field(gamefield_t *g, int *field);

WINDOW* get_gamefield_win(gamefield_t *g);

int is_row_full(int *field, int row);

int is_row_empty(int *field, int row);

void flip_values(gamefield_t *field, int rows);

void flip_values_in_row(gamefield_t *field, int row);

int check_field(gamefield_t* gameField);

int *clone_field(int *f);

#endif