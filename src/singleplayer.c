#include <ncurses.h>
#include "player.c"

Player *player;
GameField *gameField;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){
     
    player = initializePlayer("nick");
    gameField = initializeGameField();
    while (1){}
}