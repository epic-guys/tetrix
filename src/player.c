#include <ncurses.h>
#include <malloc.h>

#include "gamefield.c"

typedef struct Player
{
    int field[FIELD_ROWS][FIELD_COLS];
    WINDOW *win;
    int cursor_pos;
} Player;

/**
 * @brief Istanzia uno struct
 * giocatore e inizializza il campo
 * con valori 0 e crea una finestra nella
 * posizione indicata.
 * 
 * @param[in] y_pos La posizione Y iniziale in cui posizionare la finestra.
 * @param[in] x_pos La posizione X iniziale in cui posizionare la finestra.
 * @return Lo struct giocatore istanziato. 
 */
Player *initializePlayer(int y_pos, int x_pos)
{
    Player *player = (Player*) malloc(sizeof(Player));
    WINDOW *w;
    size_t i, j;
    for (i = 0; i < FIELD_ROWS; i++)
        for (j = 0; j < FIELD_COLS; j++)
            player->field[i][j] = 0;
    w = newwin(FIELD_W_ROWS, FIELD_W_COLS, y_pos, x_pos);
    wborder(w, '|', '|', ' ', '=', ' ', ' ', '\\', '/');
    wrefresh(w);
    player->win = w;
    player->cursor_pos = 4;
    return player;
}

/**
 * @brief Da chiamare per visualizzare
 * le modifiche al campo del giocatore.
 * 
 * @param[in, out] p Il giocatore di cui bisogna aggiornare lo schermo.
 */
void refreshPlayer(Player *p)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            wmove(p->win, FIELD_W_ROWS - FIELD_ROWS + i - 1, j * 2 + 1);
            if (p->field[i][j]){
                wattron(p->win, COLOR_PAIR(p->field[i][j]));
                wprintw(p->win, "[]");
                wattroff(p->win, COLOR_PAIR(p->field[i][j]));
            }else
                wprintw(p->win, "  ");
        }
    }
    wrefresh(p->win);
}
