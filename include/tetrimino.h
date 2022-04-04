#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

typedef enum tetrimino_type tetrimino_code_t;

typedef struct Tetrimino tetrimino_t;

tetrimino_t *getTetrimino(int type);

void freeTetrimino(tetrimino_t *t);

void printTetrimino(WINDOW *w,tetrimino_t *t,int y,int x);

typedef struct TetriminiPool tetrimini_pool_t;

tetrimini_pool_t *initializePool(int x, int y);

void freePool(tetrimini_pool_t* p);

void printMenuStyle(int i, tetrimini_pool_t *pool);

int selectTetrimino(tetrimini_pool_t *pool);

void safeRotateTetrimino(tetrimino_t *t, int cur_pos);

void refreshPool(tetrimini_pool_t *p);

void removeTetriminoFromPool(int i, tetrimini_pool_t *pool);

void addTetriminoFromPool(int i, tetrimini_pool_t *p);

int noTetriminosLeft(tetrimini_pool_t *pool);

WINDOW *getPoolWin(tetrimini_pool_t *t);

int getTetRows(tetrimino_t *t);

int getTetCols(tetrimino_t *t);

int *getTetValues(tetrimino_t *t);

int getTetType(tetrimino_t *t);

int getTetColor(tetrimino_t *t);

#endif
