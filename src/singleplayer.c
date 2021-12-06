#include <ncurses.h>
#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>

player_t *player;
player_t *player2;
gamefield_t *gameField;
tetriminipool_t *pool;
pointboard_t *points;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){

    /*TODO: Accetta l'input del nickname*/

    player = initializePlayer("nick");
    gameField = initializeGameField(12, (COLS/2)-(POOL_COLS/2)+(POOL_COLS/4));
    pool = initializePool(10, 4);
    points = initializePointBoard(10, COLS - 30, player, NULL);
    selectTetrimino(getPoolWin(pool));
    
    while (1){}
}