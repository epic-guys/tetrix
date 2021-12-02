#include <ncurses.h>
#include <player.h>
#include <gamefileld.h>
#include <tetrimino.h>

Player *player;
GameField *gameField;
TetriminiPool *pool;
PointBoard *points;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){
    player = initializePlayer("nick");
    gameField = initializeGameField(10, 4);
    pool = initializePool(10, 55);
    points = initializePointBoard(10, 10, player, NULL);
    selectTetrimino(getPoolWin(pool));
    
    while (1){}
}