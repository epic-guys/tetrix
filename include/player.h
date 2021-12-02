#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct Player Player;
typedef struct PointBoard PointBoard;

Player *initializePlayer(char *nick);
PointBoard *initializePointBoard(int y, int x, Player *player_1, Player *player_2);

#endif