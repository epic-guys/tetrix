#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct Player player_t;

player_t *initialize_player(char *nick);

void free_player(player_t* p);

typedef struct PointBoard pointboard_t;

pointboard_t *initialize_pointboard(int y, int x, player_t *player_1, player_t *player_2);

void free_pointboard(pointboard_t* p);

WINDOW* get_pointboard_win(pointboard_t *p);

void player_add_points(player_t *p,pointboard_t* board, int points);

char* get_player_nick(player_t* p);

int getPlayerPoints(player_t* p);

#endif