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

    player = initializePlayer(&nickname);
    gameField = initializeGameField(12, (COLS/2)-(POOL_COLS/2)+(POOL_COLS/4));
    pool = initializePool(10, 4);
    points = initializePointBoard(10, COLS - 30, player, NULL);
    continue_game();
}

/**
 * @brief corpo principale del gioco che si alterna fra scelta blocco e drop dello stesso
 */
void continue_game(){
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
                    if (get_tet_cols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case KEY_LEFT:
                    if (cursor > 0)
                        --cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case 'r':
                    /*ruota matrice di 90 gradi*/
                    /*mvprintw(2,3,"%d",get_tet_type(selected_t)); LA LASCIO PER COMODITÁ*/
                    selected_t = safeRotateTetrimino(selected_t, cursor);
                    /*mvprintw(6,3,"%d",get_tet_type(selected_t)); LA LASCIO PER COMODITÁ*/
                    refreshSelector(gameField, selected_t, cursor);
                break;
                case '\n':
                    dropping = 0;
                    clearTop(gameField);
                    addTetriminoToGameField(gameField,selected_t,cursor);
                    refreshGamefield(gameField);
                    mvprintw(6,9, "sono qua");
                    refresh();
                    break;
            }
            
        }
    }
}

void end_game()/*thanos++*/
{
    /* TODO */
    /*free(nickname); PER RICORDARSI, ALTRIMENTI FACCIAMO UN MEMORY LEAK*/
}