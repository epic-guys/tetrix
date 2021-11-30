#include <ncurses.h>
#include <player.h>
#include <gamefileld.h>

Player *player;
GameField *gameField;


/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){
     
    player = initializePlayer("nick");
    gameField = initializeGameField(10, 4);
    while (1){}
}