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
    WINDOW *win;
    Player *player_1;
    Player *player_2;

}PointBoard;

void refreshPointBoard(PointBoard *board)
{
    mvwprintw(board->win, 2, 2, "%-20s%3d", board->player_1->nickname, board->player_1->points);
    if (board->player_2)
    {
        mvwprintw(board->win, 4, 2, "%-20s%3d", board->player_2->nickname, board->player_2->points);
    }
    wrefresh(board->win);
}


PointBoard *initializePointBoard(int y, int x, Player *player_1, Player *player_2)
{
    PointBoard *pointBoard = (PointBoard *)malloc(sizeof(PointBoard));

    WINDOW *w;
    w = newwin(player_2 ? 7 : 5, 27, y, x);
    box(w, 0, 0);
    
    pointBoard->win = w;
    pointBoard->player_1 = player_1;
    pointBoard->player_2 = player_2;
    refreshPointBoard(pointBoard);

    return pointBoard;
}
