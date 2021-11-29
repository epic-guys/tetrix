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
    gameField = initializeGameField(0, 0);
    while (1){}
}