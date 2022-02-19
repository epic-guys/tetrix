#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct Player player_t;

player_t *initializePlayer(char *nick);

void freePlayer(player_t* p);

typedef struct PointBoard pointboard_t;

pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2);

void freePointBoard(pointboard_t* p);

WINDOW* getPointBoardWin(pointboard_t *p);

void playerAddPoints(player_t *p,pointboard_t* board, int points);

char* getPlayerNick(player_t* p);

int getPlayerPoints(player_t* p);

#endif