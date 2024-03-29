#include <stdlib.h>
#include <ncurses.h>

#include "../include/tetrimino.h"
#include "../include/gamefield.h"
#include "../include/constants.h"
#include "../include/functions.h"
#include "../include/player.h"

/**
 * @brief struct contentente il puntatore alla matrice del campo di gioco
 * e il puntatore alla finestra di ncurses.
 */
typedef struct GameField
{
    int *field;
    WINDOW *win;
} gamefield_t;

/**
 * @brief crea il campo da gioco
 *
 * @param[in] y_pos La posizione Y iniziale in cui posizionare la finestra.
 * @param[in] x_pos La posizione X iniziale in cui posizionare la finestra.
 * 
 * @return Lo struct del gamefield istanziato.
 */
gamefield_t *initialize_gamefield(int y, int x)
{

    gamefield_t *gameField = (gamefield_t *)malloc(sizeof(gamefield_t));
    gameField->field = (int *)malloc(sizeof(int) * (FIELD_COLS * FIELD_ROWS));
    WINDOW *w;
    size_t i, j;
    for (i = 0; i < FIELD_ROWS; i++)
        for (j = 0; j < FIELD_COLS; j++)
            gameField->field[FIELD_COLS * i + j] = 0;
    w = newwin(FIELD_W_ROWS, FIELD_W_COLS, y, x);
    /* ncurses con il valore 0 mette il bordo di default */
    wborder(w, 0, 0, ' ', 0, ' ', ' ', 0, 0);
    gameField->win = w;
    clear_top(gameField);
    wrefresh(w);
    return gameField;
}

/**
 * @brief free del campo da gioco.
 *
 * @param[in] g il campo da gioco.
 */
void free_gamefield(gamefield_t *g)
{
    free(g->field);
    free(g);
}

/**
 * @brief Pulizia della parte superiore dello schermo.
 *
 * @param[in] g Il campo di gioco.
 */
void clear_top(gamefield_t *g)
{
    int i, j;
    for (i = 0; i < TOP_ROWS; ++i)
    {
        wmove(g->win, i, 0);
        wclrtoeol(g->win);
    }
    wrefresh(g->win);
}
/**
 * @brief stampa l'ombra del tetramino dove questo verrá droppato. 
 * 
 * @param[in] g il puntatore al campo da gioco.
 * @param[in] t il puntatore al tetramino.
 * @param[in] cur_pos la posizione del cursore.
 */
void draw_cursor(gamefield_t *g, tetrimino_t *t, int cur_pos)
{
    int free = get_first_free_row(g->field, t, cur_pos);
    int i, j;
    int *values = get_tet_values(t);
    int rows = get_tet_rows(t);
    int cols = get_tet_cols(t);
    int color = swap_color(get_tet_color(t));
    for (i = free; i < free + rows; ++i)
    {
        for (j = cur_pos; j < cur_pos + cols; ++j)
        {
            if (values[(i - free) * cols + (j - cur_pos)])
            {
                wattron(g->win, COLOR_PAIR(color));
                mvwprintw(g->win, i + TOP_ROWS, (j * 2) + 1, "  ");
                wattroff(g->win, COLOR_PAIR(color));
            }
        }
    }
    wrefresh(g->win);
}

/**
 * @brief visualizza il tetramino selezionato nella parte superiore di un campo da gioco partendo dalla posizione del cursore.
 *
 * @param[in] g il campo da gioco dove stampare.
 * @param[in] t tetramino da stampare.
 * @param[in] cur_pos posizione del cursore.
 */
void refresh_selector(gamefield_t *g, tetrimino_t *t, int cur_pos)
{
    int i, j, val, stop = 0;
    int *values = get_tet_values(t);
    int rows = get_tet_rows(t);
    int cols = get_tet_cols(t);
    int color = get_tet_color(t);

    /*Rimuove l'overlay precente*/
    refresh_gamefield(g);

    refresh();
    /*Aggiunge l'overlay alla colonna*/

    draw_cursor(g, t, cur_pos);

    clear_top(g);

    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            val = values[(cols * i) + j];
            if (val)
            {
                wattron(g->win, COLOR_PAIR(color));
                mvwprintw(g->win, TOP_ROWS - rows + i, (cur_pos + j) * 2 + 1, "[]");
                wattroff(g->win, COLOR_PAIR(color));
            }
        }
    }

    wrefresh(g->win);
}

/**
 * @brief Gestisce l'input del drop del tetramino.
 *
 * @param[in] gameField Il campo dove bisogna droppare il tetramino.
 * @param[in] t Il tetramino da droppare.
 * 
 * @return il cursore dove droppare il tetramino.
 */
int manage_drop(gamefield_t *gameField, tetrimino_t *t)
{
    int dropping = 1;
    int cursor = (FIELD_COLS - get_tet_cols(t)) / 2;
    refresh_selector(gameField, t, cursor);
    while (dropping)
    {
        int ch = getch();
        switch (ch)
        {
        case KEY_RIGHT:
            /*Muove il tetramino a destra*/
            if (get_tet_cols(t) + cursor < FIELD_COLS)
                ++cursor;
            refresh_selector(gameField, t, cursor);

            break;
        case KEY_LEFT:
            /*Muove il tetramino a sinistra*/
            if (cursor > 0)
                --cursor;
            refresh_selector(gameField, t, cursor);
            break;

        case KEY_UP:
            /*ruota matrice di 90 gradi*/
            safe_rotate_tetrimino(t, cursor, 0);
            refresh_selector(gameField, t, cursor);
            break;

        case KEY_DOWN:
            /*ruota matrice di 90 gradi*/
            safe_rotate_tetrimino(t, cursor, 1);
            refresh_selector(gameField, t, cursor);
            break;

        case '\n': /* non é un typo */
        case KEY_ENTER:
            /*Droppa il tetramino*/
            dropping = 0;
            clear_top(gameField);
            refresh_gamefield(gameField);
            break;

        case 127: /* non é un typo */
        case KEY_BACKSPACE:
            /* Annulla la selezione */
            clear_top(gameField);
            refresh_gamefield(gameField);
            dropping = 0;
            cursor = -1;
            break;
        }
    }
    return cursor;
}

/**
 * @brief Da chiamare per visualizzare
 * le modifiche al campo del giocatore.
 *
 * @param[in] g Il campo da gioco di cui bisogna aggiornare lo schermo.
 */
void refresh_gamefield(gamefield_t *g)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            wmove(g->win, FIELD_W_ROWS - FIELD_ROWS + i - 1, j * 2 + 1);
            if (g->field[FIELD_COLS * i + j])
            {
                wattron(g->win, COLOR_PAIR(g->field[FIELD_COLS * i + j]));
                wprintw(g->win, "[]");
                wattroff(g->win, COLOR_PAIR(g->field[FIELD_COLS * i + j]));
            }
            else
                wprintw(g->win, "  ");
        }
    }
    wrefresh(g->win);
}

/**
 * @brief restituisce la prima riga libera nel campo da gioco.
 *
 * @param[in] g Il campo da gioco da controllare.
 * @param[in] t Il tetramino da incastrare.
 * @param[in] cur_pos La posizione del cursore e quindi la prima colonna di discesa del tetramino.
 * 
 * @return row La posizione nella matrice della prima riga libera.
 */
int get_first_free_row(int *f, tetrimino_t *t, int cur_pos)
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
            for (k = cur_pos; k < cur_pos + cols; ++k)
            {
                /*
                Controllo se nella posizione [j][k - cur_pos] c'è un blocco
                del tetramino. Se c'è allora controlla se c'è un blocco nella
                riga i + j esima nel campo, colonna k.
                Se c'è vuol dire che la riga non è valida, e restituisce la
                riga precedente.
                */
                if (values[j * cols + k - cur_pos])
                    if (f[FIELD_COLS * (i + j) + k])
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
 * @brief restituisce la prima riga libera nel campo da gioco.
 *
 * @param[in] g Il campo da gioco da controllare.
 * @param[in] t Il tetramino da incastrare.
 * @param[in] cur_pos La posizione del cursore e quindi la prima colonna di discesa del tetramino.
 * 
 * @return row La posizione nella matrice della prima riga libera.
 */
int get_first_free_row_in_field(int *f, int cur_pos)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            if (f[i * FIELD_COLS + j] != 0)
            {
                return i - 1;
            }
        }
    }
    return FIELD_ROWS - 1;
}

/**
 * @brief Aggiunge un tetramino al campo da gioco (funzione stub creata per compatibilitá).
 *
 * @param[in] g campo di gioco.
 * @param[in] t tetramino da piazzare.
 * @param[in] cur_pos posizione del cursore.
 * 
 * @return 1 se il tetramino si è incastrato, 0 se il
 * campo era pieno e non è riuscito.
 */
int add_tetrimino_to_gamefield(gamefield_t *g, tetrimino_t *t, int cur_pos)
{
    int res = add_tetrimino_to_field(g->field, t, cur_pos);
    refresh_gamefield(g);

    return res;
}

/**
 * @brief Aggiunge un tetramino al campo da gioco.
 *
 * @param[in] f matrice di gioco (quella dove sono salvati i tetramini).
 * @param[in] t tetramino da piazzare.
 * @param[in] cur_pos posizione del cursore.
 * 
 * @return 1 se il tetramino si è incastrato, 0 se il
 * campo era pieno e non è riuscito.
 */
int add_tetrimino_to_field(int *f, tetrimino_t *t, int cur_pos)
{
    int i = get_first_free_row(f, t, cur_pos);
    int k, l;
    int *values;
    int cols = get_tet_cols(t);
    int rows = get_tet_rows(t);
    values = get_tet_values(t);

    /*
    Se la prima riga libera è minore di zero, vuol dire che sono fuori
    dal campo, quindi il giocatore ha perso.
    */
    if (i < 0)
    {
        return 0;
    }

    /*
        PROMEMORIA:
        accedere a matrice[i][j] significa accedere alla locazione di memoria in posizione COLS ∗ 𝑖 + 𝑗
        int a[10][20]; => a[3][8] == a[3 * 20 + 8];
    */

    for (k = 0; k < rows; ++k)
    {
        for (l = 0; l < cols; ++l)
        {
            if (values[k * cols + l])
            {
                f[FIELD_COLS * (i + k) + (cur_pos + l)] = values[k * cols + l];
            }
        }
    }

    return 1;
}

/**
 * @brief controlla se il gamefield é stato
 * saturato in almeno un blocco (funzione stub).
 * 
 * @param[in] g gamefield da controllare.
 * 
 * @return 1 se é pieno, 0 altrimenti.
 */
int is_gamefield_top_occupied(gamefield_t *g)
{
    return is_field_top_occupied(g->field);
}

/**
 * @brief controlla se il field é stato
 * saturato in almeno un blocco.
 * 
 * @param[in] f la matrice campo da gioco
 * 
 * @return 1 se é pieno, 0 altrimenti.
 */
int is_field_top_occupied(int *f)
{
    return !is_row_empty(f, 0);
}

/**
 * @brief metodo getter per ricevere la matrice con i tetramini.
 *
 * @param[in] g puntatore alla struct del campo da gioco.
 * 
 * @return field la matrice di interi che rappresenta la matrice con i tetramini.
 */
int *get_gamefield(gamefield_t *g)
{
    return g->field;
}

/**
 * @brief metodo getter per ricevere la finestra ncurses del campo da gioco.
 *
 * @param[in] g puntatore alla struct del campo da gioco.
 * 
 * @return win la finestra di ncurses del campo da gioco.
 */
WINDOW *get_gamefield_win(gamefield_t *g)
{
    return g->win;
}

/**
 * @brief Controlla se la riga del campo, entrambi specificati
 * via parametro, è piena.
 *
 * @param[in] field Il campo su cui controllare la riga.
 * @param[in] row La riga da controllare.
 * 
 * @return 1 se la riga è piena, 0 se ha almeno una cella vuota, -1 se la riga
 * è fuori dal campo.
 */
int is_row_full(int *field, int row)
{
    int i;
    if (row < 0 || row >= 15)
        return -1;
    for (i = 0; i < FIELD_COLS; ++i)
    {
        if (!field[FIELD_COLS * row + i])
            return 0;
    }
    return 1;
}

/**
 * @brief Controlla se la riga del campo, entrambi specificati
 * via parametro, è vuota.
 *
 * @param[in] field Il campo su cui controllare la riga
 * @param[in] row La riga da controllare.
 * 
 * @return 1 se la riga è vuota, 0 se ha almeno una cella piena, -1 se la riga
 * è fuori dal campo.
 */
int is_row_empty(int *field, int row)
{
    int i;
    if (row < 0 || row >= FIELD_ROWS)
        return -1;
    for (i = 0; i < FIELD_COLS; ++i)
    {
        if (field[FIELD_COLS * row + i])
            return 0;
    }
    return 1;
}

/**
 * @brief Controlla il campo se è pieno o se ha
 * righe piene.
 *
 * @param[in] gameField Il campo da controllare.
 * 
 * @return Il numero di righe che il giocatore ha riempito.
 */
int check_field(gamefield_t *gameField)
{
    int i, j, deletedRows = 0;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        if (is_row_full(gameField->field, i))
        {
            int k, l;
            mvwprintw(gameField->win, i + TOP_ROWS, 1, "====================");
            wrefresh(gameField->win);
            delay(100);
            deletedRows++;
            for (k = i; k > 0; --k)
            {
                for (l = 0; l < FIELD_COLS; ++l)
                {
                    gameField->field[FIELD_COLS * k + l] = gameField->field[FIELD_COLS * (k - 1) + l];
                }
                refresh_gamefield(gameField);
                delay(50); /*la funzione in realtà blocca di fatti tutto il programma per 50 millisecondi*/
            }
        }
    }
    return deletedRows;
}

/**
 * @brief Inverte il numero di righe specificate nel
 * campo specificato.
 *
 * @param[in] field Il campo su cui invertire le righe.
 * @param[in] rows Il numero di righe.
 */
void flip_values(gamefield_t *field, int rows)
{
    int i;
    for (i = FIELD_ROWS - 1; i > FIELD_ROWS - rows - 1; --i)
    {
        flip_values_in_row(field, i);
    }
}

/**
 * @brief inverte i valori in una riga del campo da gioco.
 *
 * @param[in] field il campo da gioco.
 * @param[in] row la riga dove effettuare l'operazione.
 */
void flip_values_in_row(gamefield_t *field, int row)
{
    int i;
    if (!is_row_empty(field->field, row))
    {
        for (i = 0; i < FIELD_COLS; ++i)
        {
            if (field->field[FIELD_COLS * row + i] == 0)
            {
                field->field[FIELD_COLS * row + i] = random_color();
                refresh_gamefield(field);
                delay(50);
            }
            else
            {
                field->field[FIELD_COLS * row + i] = 0;
            }
        }
    }
}

/**
 * @brief Clona il field e restituisce il puntatore.
 * 
 * @param[in] f Il field da clonare.
 * @return La matrice clonata.
 */
int *clone_field(int *f)
{
    int *field = (int *)malloc(sizeof(int) * FIELD_ROWS * FIELD_COLS);
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            field[i * FIELD_COLS + j] = f[i * FIELD_COLS + j];
        }
    }
    return field;
}

void change_field(int** old_field,int* new_field)
{
    int* tmp = *old_field;
    *old_field = new_field;
    free(tmp);
}