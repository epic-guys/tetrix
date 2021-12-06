#include <ncurses.h>
#include <malloc.h>

#define PLAYER_NICK_LEN 20

typedef struct player
{
    int cursor_pos;
    int points;
    char *nickname;
} player_t;

/**
 * @brief Istanzia uno struct
 * giocatore e lo inizializza con il nickname
 * passato e con cursore e punti a 0.
 * @param[in] nick Il nome del giocatore.
 * @return Lo struct giocatore istanziato. 
 */
player_t *initializePlayer(char* nick)
{
    player_t *player = (player_t*) malloc(sizeof(player_t));
    player->cursor_pos = 0;
    player->points = 0;
    player->nickname = nick;
    return player;
}

typedef struct PointBoard
{
    WINDOW *win;
    player_t *player_1;
    player_t *player_2;

}pointboard_t;

void refreshPointBoard(pointboard_t *board)
{
    mvwprintw(board->win, 2, 2, "%-20s%3d", board->player_1->nickname, board->player_1->points);
    if (board->player_2)
    {
        mvwprintw(board->win, 4, 2, "%-20s%3d", board->player_2->nickname, board->player_2->points);
    }
    wrefresh(board->win);
}


pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2)
{
    pointboard_t *pointBoard = (pointboard_t *)malloc(sizeof(pointboard_t));

    WINDOW *w;
    w = newwin(player_2 ? 7 : 5, 27, y, x);
    box(w, 0, 0);
    
    pointBoard->win = w;
    pointBoard->player_1 = player_1;
    pointBoard->player_2 = player_2;
    refreshPointBoard(pointBoard);

    return pointBoard;
}

#pragma region GETTERS

int get_player_cursor_pos(player_t *p)
{
    return p->cursor_pos;
}

#pragma endregion