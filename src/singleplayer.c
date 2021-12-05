#include <ncurses.h>
#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>

Player *player;
Player *player2;
GameField *gameField;
TetriminiPool *pool;
PointBoard *points;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){

    /*TODO: Accetta l'input del nickname*/

    player = initializePlayer("nick");
    gameField = initializeGameField(12, 4);
    pool = initializePool(10, (COLS/2)-(POOL_COLS/2));
    points = initializePointBoard(10, COLS - 30, player, NULL);
    selectTetrimino(getPoolWin(pool));
    
    while (1){}
}