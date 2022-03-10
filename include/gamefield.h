#include <tetrimino.h>

#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

typedef struct GameField gamefield_t;

gamefield_t *initializeGameField(int y, int x);

void freeGamefield(gamefield_t* g);

void clearTop(gamefield_t *g);

void refreshSelector(gamefield_t *g, tetrimino_t *t, int cur_pos);

void refreshGamefield(gamefield_t *g);

void addTetriminoToGameField(gamefield_t *g,tetrimino_t *t,int cur_pos);

int gameFieldTopIsOccupied(gamefield_t* g);

int* getGamefield(gamefield_t *g);

WINDOW* getGamefieldWin(gamefield_t *g);

int is_row_empty(gamefield_t *field, int row);

#endif