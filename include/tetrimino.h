#ifndef _TETRIMINO_H_
#define _TETRIMINO_H_

typedef enum TetriminoType TetriminoType;

typedef struct Tetrimino Tetrimino;

Tetrimino getTetrimino(TetriminoType type);

typedef struct TetriminoSet TetriminoSet;

#endif
