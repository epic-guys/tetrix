#include <ncurses.h>
#include <player.h>
#include <gamefileld.h>
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
    player = initializePlayer("nick");
    gameField = initializeGameField(10, 4);
    pool = initializePool(10, 50);
    points = initializePointBoard(10, 104, player, NULL);
    selectTetrimino(getPoolWin(pool));
    
    while (1){}
}