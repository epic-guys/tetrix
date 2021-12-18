#include <tetrimino.h>

#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

typedef struct GameField gamefield_t;

gamefield_t *initializeGameField(int y, int x);
void clearTop(gamefield_t *g);
void refreshGamefield(gamefield_t *g);
void refreshSelector(gamefield_t *g, tetrimino_t *t, int cur_pos);
void addTetriminoToGameField(gamefield_t *g,tetrimino_t *t,int cur_pos);

#endif