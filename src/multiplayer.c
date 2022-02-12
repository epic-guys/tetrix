#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <constants.h>

void continue_game();
void end_game();

/**
 * @brief Inizia la partita in multiplayer
 * 
 */
void newGameMulti(){
    player_t *player1;
    player_t *player2;
    tetramini_pool_t *pool;
    pointboard_t *points;

    char *nickname1 = (char*) calloc(sizeof(char),16);
    form(nickname1, 16, " Nome: ");
    refresh();

    char *nickname2 = (char*) calloc(sizeof(char),16);
    form(nickname2, 16, " Nome: ");
    refresh();

    player1 = initializePlayer(nickname1);
    player2 = initializePlayer(nickname2);
    gameField = initializeGameField(12, (COLS/2)-(POOL_COLS/2)+(POOL_COLS/4));
    pool = initializePool(10, 4);
    points = initializePointBoard(10, COLS - 30, player, NULL);
}