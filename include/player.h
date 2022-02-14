#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct Player player_t;
typedef struct PointBoard pointboard_t;

player_t *initializePlayer(char *nick);
void freePlayer(player_t* p);
pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2);
void freePointBoard(pointboard_t* p);

#pragma region GETTERS & SETTERS

int get_player_cursor_pos(player_t *p);
void set_player_cursor_pos(player_t *p, int cursor_pos);
void playerAddPoints(player_t *p,pointboard_t* board, int points);
WINDOW* getPointBoardWin(pointboard_t *p);
char* getPlayerNick(player_t* p);
int getPlayerPoints(player_t* p);

#pragma endregion

#endif