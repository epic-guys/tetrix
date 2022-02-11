#include <stdlib.h>
#include <ncurses.h>
#include <tetrimino.h>
#include <gamefield.h>
#include <constants.h>

typedef struct GameField
{
    int field[FIELD_ROWS][FIELD_COLS];
    WINDOW *win;
} gamefield_t;

/**
 * @brief crea il campo da gioco
 * @param[in] y_pos La posizione Y iniziale in cui posizionare la finestra.
 * @param[in] x_pos La posizione X iniziale in cui posizionare la finestra.
 */
gamefield_t *initializeGameField(int y, int x){

    gamefield_t *gameField = (gamefield_t*) malloc(sizeof(gamefield_t));
    WINDOW *w;
    size_t i, j;
    for (i = 0; i < FIELD_ROWS; i++)
        for (j = 0; j < FIELD_COLS; j++)
            gameField->field[i][j] = 0;
    w = newwin(FIELD_W_ROWS, FIELD_W_COLS, y, x);
    /* ncurses con il valore 0 mette il bordo di default */
    wborder(w, 0, 0, ' ', 0, ' ', ' ', 0, 0);
    gameField->win = w;
    clearTop(gameField);
    wrefresh(w);
    return gameField;
}

/**
 * @brief Pulizia della parte superiore dello schermo
 * @param[in] g Il campo di gioco
 */
void clearTop(gamefield_t *g){
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        wmove(g->win, i, 0);
        wclrtoeol(g->win);
    }
    wrefresh(g->win);
}


/**
 * @brief stampa il tetramino selezionato nella parte superiore di un campo da gioco partendo dalla posizione del cursore
 * @param[in] g il campo da gioco dove stampare
 * @param[in] t tetramino da stampare
 * @param[in] cur_pos posizione del cursore
 */
void refreshSelector(gamefield_t *g, tetrimino_t *t, int cur_pos)
{
    int i, j, val;
    clearTop(g);
    int *values = get_tet_values(t);

    for (i = 0; i < get_tet_rows(t); ++i)
    {
        for (j = 0; j < get_tet_cols(t); ++j)
        {
            val = values[(get_tet_cols(t)*i)+j];
            if (val)
            {
                wattron(g->win, COLOR_PAIR(val));
                mvwprintw(g->win, 4 - get_tet_rows(t) + i, (cur_pos + j) * 2 + 1, "[]");
                /* wwattroff(g->win, COLOR_PAIR(g->field[i][j])); ALVI PO MI SPIEGHI CHE STAVI PENSANDO DI FARE CON QUESTA RIGA LOL, BUGGAVA TUTTOattroff(g->win, COLOR_PAIR(g->field[i][j])); ALVI PO MI SPIEGHI CHE STAVI PENSANDO DI FARE CON QUESTA RIGA LOL, BUGGAVA TUTTO*/
            }
        }
    }

    wrefresh(g->win);
}


/**
 * @brief Da chiamare per visualizzare
 * le modifiche al campo del giocatore.
 * 
 * @param[in, out] g Il campo da gioco di cui bisogna aggiornare lo schermo.
 */
void refreshGamefield(gamefield_t *g)
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

int get_first_free_row(gamefield_t *g,tetrimino_t *t,int cur_pos)
{
    int i, j, k;
    int *values = get_tet_values(t);
    int cols = get_tet_cols(t);
    int rows = get_tet_rows(t);

    /*
    Scannerizzo il campo da gioco per i righe.
    Dato che si considera la riga superiore del tetramino,
    per evitare che vada fuori dal campo si escludono le ultime
    rows - 1 righe del campo.
    */
    for (i = 0; i < FIELD_ROWS - rows + 1; ++i)
    {
        /*
        A questo punto scansiono le righe del tetramino.
        E tu starai pensando: "ma le colonne?" adesso ci arriviamo
        con il prossimo for.
        */
        for (j = 0; j < rows; ++j)
        {
            /*
            Scansiono sia le colonne del campo sia quelle del tetramino
            contemporaneamente. Facendo così si evita di usare un quarto
            ciclo for e si evitano problemi di andare fuori memoria degli array.
            */
            for (k = cur_pos; k < cur_pos + cols ; ++k)
            {
                /*
                Controllo se nella posizione [j][k - cur_pos] c'è un blocco
                del tetramino. Se c'è allora controlla se c'è un blocco nella
                riga i + j esima nel campo, colonna k.
                Se c'è vuol dire che la riga non è valida, e restituisce la
                riga precedente.
                */
                if (values[j * cols + k - cur_pos])
                    if (g->field[i + j][k])
                        return i - 1;
            }
        }
    }
    /*
    Se sei arrivato fino a qui hai scorso tutte
    le righe del campo, quindi lo lasci fino in fondo.
    */
    return i - 1;
}

/**
 * @brief aggiunge un tetramino al campo da gioco
 * @param[in] g campo di gioco
 * @param[in] t tetramino da piazzare
 * @param[in] cur_pos posizione del cursore
 */
void addTetriminoToGameField(gamefield_t *g,tetrimino_t *t,int cur_pos){
    int i = get_first_free_row(g, t, cur_pos);
    int k, l;
    int *values;
    int cols=get_tet_cols(t);
    int rows=get_tet_rows(t);
    values=get_tet_values(t);

    /*accedere a matrice[i][j] significa accedere alla locazione di memoria in posizione COLS ∗ 𝑖 + 𝑗
      int a[10][20]; => a[3][8] == a[3 * 20 + 8];    
    */

   mvprintw(3,4,"                         ");
   refresh();


    for (k = 0; k < rows; ++k)
    {
        for (l = 0; l < cols; ++l)
        {
            if (values[k * cols + l])
            {
                g->field[i + k][cur_pos + l] = values[k * cols + l];
            }
        }
    }

}
