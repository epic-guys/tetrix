#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

#define POOL_ROWS 23
#define POOL_COLS 50

typedef enum TetriminoType TetriminoType;

typedef struct Tetrimino Tetrimino;

Tetrimino getTetrimino(TetriminoType type);

typedef struct TetriminoSet TetriminoSet;

typedef struct TetriminiPool TetriminiPool;

TetriminiPool *initializePool(int x, int y);

WINDOW *getPoolWin(TetriminiPool *t);

int selectTetrimino(WINDOW *W);

void printTetrimino(WINDOW *w,Tetrimino t,int y,int x);

#endif
