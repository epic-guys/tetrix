#include <ncurses.h>
#include <player.h>
#include <gamefileld.h>
#include <tetrimino.h>

Player *player;
GameField *gameField;
TetriminiPool *pool;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){
     
    player = initializePlayer("nick");
    gameField = initializeGameField(10, 4);
    pool = initializePool(55, 10);
    while (1){}
}