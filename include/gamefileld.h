#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

typedef struct GameField GameField;

GameField *initializeGameField(int y, int x);
void clearTop(GameField *g);
void refreshGamefield(GameField *g);

#endif