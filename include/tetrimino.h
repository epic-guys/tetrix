#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

typedef enum TetriminoType TetriminoType;

typedef struct Tetrimino Tetrimino;

Tetrimino getTetrimino(TetriminoType type);

typedef struct TetriminoSet TetriminoSet;

typedef struct TetriminiPool TetriminiPool;

TetriminiPool *initializePool(int x, int y);


#endif
