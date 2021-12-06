#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct player_t player_t;
typedef struct pointboard_t pointboard_t;

player_t *initializePlayer(char *nick);
pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2);

#endif