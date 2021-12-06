#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct PLayer player_t;
typedef struct PointBoard pointboard_t;

player_t *initializePlayer(char *nick);
pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2);

#pragma region GETTERS

int get_player_cursor_pos(player_t *p);

#pragma endregion

#endif