#include <stdlib.h>
#include <ncurses.h>
#include <tetrimino.h>
#include <gamefield.h>

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
                /* wattroff(g->win, COLOR_PAIR(g->field[i][j])); ALVI PO MI SPIEGHI CHE STAVI PENSANDO DI FARE CON QUESTA RIGA LOL, BUGGAVA TUTTO*/
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

/**
 * @brief aggiunge un tetramino al campo da gioco
 * SPOILER: NON FUNZIONA, LOOOOOOOL
 * @param[in] g campo di gioco
 * @param[in] t tetramino da piazzare
 * @param[in] cur_pos posizione del cursore
 */
void addTetriminoToGameField(gamefield_t *g,tetrimino_t *t,int cur_pos){
    int i,j,k,l,cols,rows,val;
    int *values;
    cols=get_tet_cols(t);
    rows=get_tet_rows(t);
    values=get_tet_values(t);

    /*Scannerizzo il campo da gioco per i righe e j colonne*/
    for(i=0;i<FIELD_ROWS;++i){
        for(j=cur_pos;j<(cur_pos + cols);++j){


            /*se sono all'ultima riga o se in quella posizione ci sta un pezzo risalgo*/
            if((g->field[FIELD_ROWS-1][j]==0 && i==FIELD_ROWS-1)|| g->field[i][j]!=0){
                if(g->field[i][j]!=0){
                    i--;
                }

                /*TODO: Verificare che il tetramino ci stia controllando tutta la riga, far si che possa fittare per davvero (se val é 0 si puó scendere)*/

                /*salgo di altezza tetramino*/
                i = i - rows+1;

                /*stampo il tetramino*/
                for(k=rows-1;k>=0;k--){
                    for(l=0;l<cols;l++){
                        g->field[i+k][j+l] = values[(get_tet_cols(t)*k+l)];
                        //g->field[i][j] = 7;
                    }
                }
                return;
            }
        }
    }

}