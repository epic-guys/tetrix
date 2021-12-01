#include <ncurses.h>
#include <malloc.h>

typedef struct Player
{
    int cursor_pos;
    int points;
    char *nickname;
} Player;

/**
 * @brief Istanzia uno struct
 * giocatore e lo inizializza con il nickname
 * passato e con cursore e punti a 0.
 * @param[in] nick Il nome del giocatore.
 * @return Lo struct giocatore istanziato. 
 */
Player *initializePlayer(char* nick)
{
    Player *player = (Player*) malloc(sizeof(Player));
    player->cursor_pos = 0;
    player->points = 0;
    player->nickname = nick;
    return player;
}

typedef struct PointBoard
{
    WINDOW *pool;
    Player *player_1;
    Player *player_2;

}PointBoard;

PointBoard *initializePointBoard(int y, int x, Player *player_1, Player *player_2)
{
    PointBoard *pointBoard = (PointBoard *)malloc(sizeof(PointBoard));
    WINDOW *w;

    w = newwin(player_2 ? 7 : 5, 25, y, x);
    box(w, 0, 0);
    //mvwprintw(w, 2, 1, )
    wrefresh(w);
    //tetriminiPool->pool = w;
    return pointBoard;
}
