#include <stdlib.h>
#include <ncurses.h>
#include <tetrimino.h>

#define FIELD_ROWS 15
#define FIELD_COLS 10
#define FIELD_W_ROWS FIELD_ROWS + 1 + 4 /*Finestra del campo->Righe(verticale) = Numero righe + 1 riga(sotto) + 4 blocco di selezione*/
#define FIELD_W_COLS FIELD_COLS * 2 + 2 /*Finestra del campo->Colonne(orizzontale = NUmero colonne * 2(blocco) + 2 bordi(destra-sinistra)*/

typedef struct GameField
{
    int field[FIELD_ROWS][FIELD_COLS];
    WINDOW *win;
} GameField;

/**
 * @brief crea il campo da gioco
 * @param[in] y_pos La posizione Y iniziale in cui posizionare la finestra.
 * @param[in] x_pos La posizione X iniziale in cui posizionare la finestra.
 */
GameField *initializeGameField(int y, int x){

    GameField *gameField = (GameField*) malloc(sizeof(GameField));
    WINDOW *w;
    size_t i, j;
    for (i = 0; i < FIELD_ROWS; i++)
        for (j = 0; j < FIELD_COLS; j++)
            gameField->field[i][j] = 0;
    w = newwin(FIELD_W_ROWS, FIELD_W_COLS, y, x);
    wborder(w, '|', '|', ' ', '=', ' ', ' ', '\\', '/');
    wrefresh(w);
    gameField->win = w;
    return gameField;
}

void clearTop(GameField *g){
    int i, j;
    for (i = 0; i < 3; ++i)
    {
        wmove(g->win, i, 0);
        wclrtoeol(g->win);
    }
}

/*
WORK IN PROGRESS
void refreshSelector(GameField *g, Tetrimino t)
{
    int i, j;
    clearTop(g);

    for (i = 0; i < t.rows; ++i)
    {
        for (j = 0; j < t.cols; ++j)
        {
            if (t.values[t.cols * i + j])
            {
                wattron(g->win, COLOR_PAIR(g->field[i][j]));
                mvwprintw(g->win, 4 - t.rows + i, (g->cursor_pos + j) * 2, "[]");
                wattroff(g->win, COLOR_PAIR(g->field[i][j]));
            }
        }
    }

    wrefresh(g->win);
}
*/

/**
 * @brief Da chiamare per visualizzare
 * le modifiche al campo del giocatore.
 * 
 * @param[in, out] g Il campo da gioco di cui bisogna aggiornare lo schermo.
 */
void refreshGamefield(GameField *g)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            wmove(g->win, FIELD_W_ROWS - FIELD_ROWS + i - 1, j * 2 + 1);
            if (g->field[i][j]){
                wattron(g->win, COLOR_PAIR(g->field[i][j]));
                wprintw(g->win, "[]");
                wattroff(g->win, COLOR_PAIR(g->field[i][j]));
            }else
                wprintw(g->win, "  ");
        }
    }
    wrefresh(g->win);
}