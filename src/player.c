#include <ncurses.h>
#include <malloc.h>

#include "gamefield.c"
#include "tetrimino.c"

typedef struct Player
{
    int cursor_pos;
    int points;
    char *nickname;
} Player;

/**
 * @brief Istanzia uno struct
 * giocatore e inizializza il campo
 * con valori 0 e crea una finestra nella
 * posizione indicata.
 * @return Lo struct giocatore istanziato. 
 */
Player *initializePlayer(char* nick)
{
    Player *player = (Player*) malloc(sizeof(Player));
    player->cursor_pos = 4;
    player->points = 0;
    player->nickname = nick;
    return player;
}