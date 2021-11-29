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