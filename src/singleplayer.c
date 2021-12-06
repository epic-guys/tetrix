#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>

player_t *player;
gamefield_t *gameField;
tetrimini_pool_t *pool;
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
    continue_game();
}

void continue_game()
{
    int selected_i, can_play = 1;
    tetrimino_t *selected_t;
    
    while (can_play)
    {
        int dropping = 1, cursor;
        selected_i = selectTetrimino(pool);
        selected_t = getTetrimino(selected_i);
        cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;
        refreshSelector(gameField, selected_t, cursor);
        while (dropping)
        {
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    if (get_tet_cols(selected_t) + cursor < 10)
                        ++cursor;
                    break;
                case KEY_LEFT:
                    if (cursor > 0)
                        --cursor;
                    break;
                case '\n':
                    dropping = 0;
                    break;
            }
            refreshSelector(gameField, selected_t, cursor);
        }
    }
}

void end_game()
{
    /* TODO */
}