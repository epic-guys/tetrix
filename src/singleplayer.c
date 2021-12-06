#include <ncurses.h>
#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>

player_t *player;
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
    int t = selectTetrimino(getPoolWin(pool));
    tetrimino_t *selected = getTetrimino(t);
    refreshSelector(gameField, selected, get_player_cursor_pos(player));
    
    while (1){}
}