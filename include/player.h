#ifndef _PLAYER_H_
#define _PLAYER_H_

#define POINTBOARD_COLS 27

typedef struct Player player_t;
typedef struct PointBoard pointboard_t;

player_t *initializePlayer(char *nick);
pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2);

#endif