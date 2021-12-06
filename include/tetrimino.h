#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

#define POOL_ROWS 23
#define POOL_COLS 50

typedef enum tetrimino_type tetrimino_code_t;

typedef struct Tetrimino tetrimino_t;

tetrimino_t *getTetrimino(int type);

typedef struct TetriminoSet tetriminoset_t;

typedef struct TetriminiPool tetrimini_pool_t;

tetrimini_pool_t *initializePool(int x, int y);

WINDOW *getPoolWin(tetrimini_pool_t *t);

int selectTetrimino(tetrimini_pool_t *pool);

void printTetrimino(WINDOW *w,tetrimino_t *t,int y,int x);

int get_tet_rows(tetrimino_t *t);

int get_tet_cols(tetrimino_t *t);

int *get_tet_values(tetrimino_t *t);

#endif
