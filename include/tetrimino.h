#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

#define POOL_ROWS 23
#define POOL_COLS 50

typedef enum TetriminoType TetriminoType;

typedef struct Tetrimino tetrimino_t;

tetrimino_t getTetrimino(TetriminoType type);

typedef struct TetriminoSet tetriminoset_t;

typedef struct TetriminoPool tetriminipool_t;

tetriminipool_t *initializePool(int x, int y);

WINDOW *getPoolWin(tetriminipool_t *t);

int selectTetrimino(WINDOW *W);

void printTetrimino(WINDOW *w,tetrimino_t t,int y,int x);

#endif
