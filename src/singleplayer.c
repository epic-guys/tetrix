#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>

player_t *player;
player_t *player2;
gamefield_t *gameField;
tetriminipool_t *pool;
pointboard_t *points;

/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){
    int i;

    char *nickname = (char*) calloc(sizeof(char),16);
    form(&nickname, 16, " Nome: ");
    refresh();
    //while (1){}

    player = initializePlayer(&nickname);
    gameField = initializeGameField(12, (COLS/2)-(POOL_COLS/2)+(POOL_COLS/4));
    pool = initializePool(10, 4);
    points = initializePointBoard(10, COLS - 30, player, NULL);
    selectTetrimino(getPoolWin(pool));
    
    while (1){}
}