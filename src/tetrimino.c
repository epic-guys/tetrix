#include <ncurses.h>
#include <malloc.h>

#include "../include/tetrimino.h"
#include "../include/gamefield.h"
#include "../include/constants.h"
#include "../include/functions.h"

/**
 * @brief Non è un typo, in inglese
 * si scrive così.
 *
 * L'idea è di salvare
 * ogni tetramino in una matrice
 * e salvarci affianco la larghezza,
 * l'altezza e il tipo,
 *  in questo modo diventa
 * più facile gestire la caduta dall'alto,
 * i bordi del campo e la rotazione.
 *
 * Inoltre il valore assegnato ad ogni campo
 * della matrice si riferisce al colore.
 * ATTENZIONE: ricordarsi ogni volta di deallocare il tetramino
 */
typedef struct Tetrimino
{
    int *values;
    int cols;
    int rows;
    tetrimino_type_t type;
} tetrimino_t;

/**
 * @brief Genera il tetramino dato il suo enum.
 *
 * @param[in] type la sua codifica.
 * @return Il tetramino allocato. È vuoto se il valore passato non è valido.
 */
tetrimino_t *get_tetrimino(int type)
{
    int *values;
    tetrimino_t *t = malloc(sizeof(tetrimino_t));
    t->type = type;
    switch (type)
    {
    case T_I:
        t->values = malloc(sizeof(int) * 1 * 4);
        t->values[0] = 1;
        t->values[1] = 1;
        t->values[2] = 1;
        t->values[3] = 1;

        t->cols = 4;
        t->rows = 1;
        break;

    case T_J:
        t->values = malloc(sizeof(int) * 2 * 3);
        t->values[0] = 2;
        t->values[1] = 0;
        t->values[2] = 0;
        t->values[3] = 2;
        t->values[4] = 2;
        t->values[5] = 2;

        t->cols = 3;
        t->rows = 2;
        break;

    case T_L:
        t->values = malloc(sizeof(int) * 2 * 3);
        t->values[0] = 0;
        t->values[1] = 0;
        t->values[2] = 3;
        t->values[3] = 3;
        t->values[4] = 3;
        t->values[5] = 3;

        t->cols = 3;
        t->rows = 2;
        break;

    case T_S:
        t->values = malloc(sizeof(int) * 2 * 3);
        t->values[0] = 0;
        t->values[1] = 5;
        t->values[2] = 5;
        t->values[3] = 5;
        t->values[4] = 5;
        t->values[5] = 0;

        t->cols = 3;
        t->rows = 2;
        break;

    case T_O:
        t->values = malloc(sizeof(int) * 2 * 2);
        t->values[0] = 4;
        t->values[1] = 4;
        t->values[2] = 4;
        t->values[3] = 4;

        t->cols = 2;
        t->rows = 2;
        break;

    case T_Z:
        t->values = malloc(sizeof(int) * 2 * 3);
        t->values[0] = 7;
        t->values[1] = 7;
        t->values[2] = 0;
        t->values[3] = 0;
        t->values[4] = 7;
        t->values[5] = 7;

        t->cols = 3;
        t->rows = 2;
        break;

    case T_T:
        t->values = malloc(sizeof(int) * 2 * 3);
        t->values[0] = 0;
        t->values[1] = 6;
        t->values[2] = 0;
        t->values[3] = 6;
        t->values[4] = 6;
        t->values[5] = 6;

        t->cols = 3;
        t->rows = 2;
        break;
    default:
        t->values = NULL;
        t->cols = 0;
        t->rows = 0;
    }
    return t;
}

/**
 * @brief Libera la memoria allocata dal tetrimino.
 *
 * @param[in, out] t Il tetrimino da deallocare.
 */
void free_tetrimino(tetrimino_t *t)
{
    free(t->values);
    free(t);
}

/**
 * @brief funzione che printa i tetramini, ATTENZIONE: SI PARTE A PRINTARE DA X0 Y0 DEL TETRAMINO
 * @param[in] w  la finestra dove printarlo
 * @param[in] t  Il tetramino da printare
 * @param[in] y  la y relativa della finestra
 * @param[in] x  la x relativa della finestra
 */
void print_tetrimino(WINDOW *w, tetrimino_t *t, int y, int x)
{
    int i, j, c = 0;
    for (i = 0; i < t->rows; ++i)
    {
        for (j = 0; j < t->cols; ++j)
        {
            if (t->values[c])
            {
                wattron(w, COLOR_PAIR(t->values[c]));
                mvwprintw(w, y + i, (x + j) * 2, "[]");
                wattroff(w, COLOR_PAIR(t->values[c]));
            }
            c++;
        }
    }
}

/**
 * @brief La struct della finestra con tutti i pezzi dei tetramini rimanenti.
 */
typedef struct TetriminiPool
{
    WINDOW *win;
    int *rem_tetriminos;
    /*
    Per ottimizzare il controllo di
    quanti tetramini sono rimasti, meglio
    fare una cache di quelli totali.
    Molto più veloce di scorrere ogni volta
    l'array.
    */
    int total_tetriminos;
} tetrimini_pool_t;

/**
 * @brief funzione per inizializzare la pool dove sono presenti tutti i tetramini rimasti.
 *
 * @param[in] y La riga da dalla quale deve iniziare a posizionare la finestra.
 * @param[in] x La colonna da dalla quale deve iniziare a posizionare la finestra.
 * @return Lo struct della pool istanziato.
 */
tetrimini_pool_t *initialize_pool(int y, int x)
{
    int i;
    WINDOW *w;
    tetrimini_pool_t *tetriminiPool = (tetrimini_pool_t *)malloc(sizeof(tetrimini_pool_t));
    tetriminiPool->rem_tetriminos = malloc(sizeof(int) * N_tetrimini);
    tetriminiPool->total_tetriminos = N_tetrimini * TETRIMINOS_PER_TYPE;
    for (i = 0; i < N_tetrimini; ++i)
    {
        tetriminiPool->rem_tetriminos[i] = TETRIMINOS_PER_TYPE;
    }

    w = newwin(POOL_ROWS, POOL_COLS, y, x);
    /* Attiva i tasti speciali, tra cui le freccette */
    keypad(w, TRUE);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
    box(w, 0, 0);
    wmove(w, getcurx(w) + 1, getcury(w) + 2);
    wprintw(w, "TETRAMINI DISPONIBILI: ");
    wrefresh(w);
    tetriminiPool->win = w;
    return tetriminiPool;
}

/**
 * @brief free della pool di tetramini.
 *
 * @param[in] p La pool.
 */
void free_pool(tetrimini_pool_t *p)
{
    free(p->rem_tetriminos);
    free(p);
}

/**
 * @brief Funzione STUB che stampa i tetramini in modalitá "menu".
 *
 * @param[in] i l'indice del menu.
 * @param[in] pool la pool nella quale stampare.
 */
void print_menu_style(int i, tetrimini_pool_t *pool)
{
    tetrimino_t *t = get_tetrimino(i);
    print_tetrimino(pool->win, t, 2 + (i * 3), 3);
    free_tetrimino(t);
    mvwprintw(pool->win, getcury(pool->win), POOL_COLS / 2, "Rimanenti:%10d", pool->rem_tetriminos[i]);
}

/**
 * @brief permette di accedere al metodo di selezione dei tetramini.
 *
 * @param [in] w Finestra della pool da cui selezionare il pezzo.
 * @return Il numero della codifica del tetramino, -1 se non ci sono
 * più tetramini.
 */
int select_tetrimino(tetrimini_pool_t *pool)
{
    int i, ch, selecting = 1;

    /* Stampa il menu iniziale */
    for (i = 0; i < N_tetrimini; ++i)
    {
        print_menu_style(i, pool);
    }

    if (no_tetriminos_left(pool))
        return -1;

    /*carica lo schermo*/
    i = -1;
    do
    {
        i = next_circular(i, N_tetrimini);
    } while (pool->rem_tetriminos[i] <= 0);
    wattron(pool->win, A_STANDOUT);
    print_menu_style(i, pool);
    wattroff(pool->win, A_STANDOUT);
    wrefresh(pool->win);
    while (selecting)
    {
        ch = wgetch(pool->win);
        print_menu_style(i, pool);
        switch (ch)
        {
        case KEY_UP:
            do
            {
                i = prev_circular(i, N_tetrimini);
            } while (pool->rem_tetriminos[i] <= 0);

            break;
        case KEY_DOWN:
            do
            {
                i = next_circular(i, N_tetrimini);
            } while (pool->rem_tetriminos[i] <= 0);
            break;
        case '\n':
            selecting = 0;
            break;
        }

        /*Sottolinea la scelta*/
        wattron(pool->win, A_STANDOUT);
        print_menu_style(i, pool);
        wattroff(pool->win, A_STANDOUT);
        wrefresh(pool->win);
    }
    if (pool->rem_tetriminos[i] > 0)
        return i;
    else
        return -1;
}

/**
 * @brief Semplice algoritmo per ruotare un tetramino, sviluppato attraverso l'algebra lineare.
 * Le posizioni dei blocchi vengono trattate come dei vettori e l'algoritmo è una applicazione
 * lineare.
 * Lo spazio vettoriale è \f$ V= \mathbb{R}^2 \f$ con base
 * \f[
 * \mathcal{B} = \left\{
 * \begin{pmatrix}
 *   1 \\
 *   0
 * \end{pmatrix},
 * \begin{pmatrix}
 *   0 \\
 *   -1
 * \end{pmatrix}
 * \right\}
 *  \f]
 *  e l'applicazione lineare \f$ T: V \rightarrow V \f$:
 *  \f[
 *  T(v)=
 *  \begin{pmatrix}
 *   -v_2 \\
 *   v_1
 *  \end{pmatrix}
 * \f]
 * con $v = (v_1, v_2)^T \in V$.
 *
 * L'applicazione lineare ha come risultato dei vettori ruotati di 90° che corrispondono
 * alle posizioni del tetramino ruotato. Dato che però in realtà si parla di indici di array
 * bisogna sommare all'indice della colonna il numero totale di colonne - 1. Senza questa
 * operazione il tetramino risulterebbe "fuori" dalle celle e avrebbe indici negativi.
 *
 * @param[in] t Il tetramino da ruotare.
 * @param[in] dir La direzione in cui ruotare il tetramino. 0 sta per destra, qualsiasi altro valore sinistra.
 */
void linear_rotate(tetrimino_t *t, int dir)
{
    int *v_rotate = (int *)malloc(sizeof(int) * t->rows * t->cols);
    size_t i, j;

    if (dir == 0)
    {
        for (i = 0; i < t->rows; ++i)
        {
            for (j = 0; j < t->cols; ++j)
            {
                int i_rot = j;
                int j_rot = -i + t->rows - 1;
                v_rotate[i_rot * t->rows + j_rot] = t->values[i * t->cols + j];
            }
        }
    }
    else
    {
        for (i = 0; i < t->rows; ++i)
        {
            for (j = 0; j < t->cols; ++j)
            {
                int i_rot = -j + t->cols - 1;
                int j_rot = i;
                v_rotate[i_rot * t->rows + j_rot] = t->values[i * t->cols + j];
            }
        }
    }
    free(t->values);
    t->values = v_rotate;
    i = t->rows;
    t->rows = t->cols;
    t->cols = i;
}

/**
 * @brief Stub di rotateLinear che verifica la fattibilitá della rotazione altrimenti non ruota.
 *
 * @param[in] t Tetramino da ruotare.
 * @param[in] cur_pos Posizione del cursore.
 * @param[in] dir Direzione di rotazione.
 * @return 1 se è riuscito a ruotare, 0 altrimenti.
 */
int safe_rotate_tetrimino(tetrimino_t *t, int cur_pos, int dir)
{

    if (cur_pos + t->rows <= FIELD_COLS)
    {
        linear_rotate(t, dir);
        return 1;
    }
    else
        return 0;
}

/**
 * @brief refresh della pool.
 *
 * @param[in] p la pool.
 */
void refresh_pool(tetrimini_pool_t *p)
{
    wrefresh(p->win);
    refresh();
}

/**
 * @brief rimuove un'unitá di un tetramino dalla pool.
 *
 * @param[in] i il tipo di tetramino dalla quale togliere un'unitá.
 * @param[in] p la pool di tetramini.
 */
void remove_tetrimino_from_pool(int i, tetrimini_pool_t *p)
{
    if (p->rem_tetriminos[i] > 0)
    {
        p->rem_tetriminos[i]--;
        p->total_tetriminos--;
    }
    refresh_pool(p);
}

/**
 * @brief aggiunge un'unitá di un tetramino dalla pool.
 *
 * @param[in] i il tipo di tetramino dalla quale aggiungere un'unitá.
 * @param[in] p la pool di tetramini.
 */
void add_tetrimino_from_pool(int i, tetrimini_pool_t *p)
{
    p->rem_tetriminos[i]++;
    p->total_tetriminos++;
    refresh_pool(p);
}

/**
 * @brief controlla se ci sono tetramini rimanenti.
 *
 * @param[in] pool la pool di tetramini selezionabili.
 *
 * @param[out] i ritorna 0 se c'é almeno un tetramino di qualsiasi tipo, 1 altrimenti
 */
int no_tetriminos_left(tetrimini_pool_t *pool)
{
    int i;
    for (i = 0; i < N_tetrimini; ++i)
    {
        if (pool->rem_tetriminos[i])
            return 0;
    }
    return 1;
}

/**
 * @brief restituisce la finestra della pool di tetramini.
 *
 * @param[in] t la pool di tetramini.
 * @param[out] win la finestra ncurses della pool.
 */
WINDOW *get_pool_win(tetrimini_pool_t *t)
{
    return t->win;
}

/**
 * @brief Restituisce il numero di tetramini rimasti
 * per il tipo passato per parametro.
 *
 * @param type Il tipo di tetrimino.
 * @return Il numero di tetramini rimasti.
 */
int get_remaining_tetriminos(tetrimini_pool_t *pool, tetrimino_type_t type)
{
    return pool->rem_tetriminos[type];
}

/**
 * @brief restituisce il numero di righe di un tetramino.
 *
 * @param[in] t il tetramino.
 * @param[out] rows il numero di righe di cui é composto il tetramino.
 */
int get_tet_rows(tetrimino_t *t)
{
    return t->rows;
}

/**
 * @brief restituisce il numero di colonne di un tetramino.
 *
 * @param[in] t il tetramino.
 * @param[out] cols il numero di colonne di cui é composto il tetramino.
 */
int get_tet_cols(tetrimino_t *t)
{
    return t->cols;
}

/**
 * @brief restituisce l'array di valori di un tetramino.
 *
 * @param[in] t il tetramino.
 * @param[out] values il puntatore all'array di valori del tetramino.
 */
int *get_tet_values(tetrimino_t *t)
{
    return t->values;
}

/**
 * @brief restituisce il tipo di tetramino.
 *
 * @param[in] t il tetramino.
 * @param[out] type l'intero che identifica il tipo di tetramino.
 *
 */
int get_tet_type(tetrimino_t *t)
{
    return t->type;
}
/**
 * @brief restituisce il colore del tetramino.
 *
 * @param[in] t il tetramino.
 * @param[out] color il numero che identifica il colore del tetramino.
 */
int get_tet_color(tetrimino_t *t)
{
    int i;
    int *values = get_tet_values(t);
    for (i = 0; i < (get_tet_cols(t) * get_tet_rows(t)) - 1; ++i)
    {
        if (values[i] != 0)
            return values[i];
    }
}

int is_safe_to_place_tet(tetrimino_t *t)
{
    int i, j, k;
    int cols, rows;
    int *values;

    cols = get_tet_cols(t);
    rows = get_tet_rows(t);
    values = get_tet_values(t);

    /*
        PROMEMORIA:
        accedere a matrice[i][j] significa accedere alla locazione di memoria in posizione COLS ∗ 𝑖 + 𝑗
        int a[10][20]; => a[3][8] == a[3 * 20 + 8];
    */
    for (j = cols - 1; j >= 0; --j)
    {
        int found_free = 0;
        for (i = rows - 1; i >= 0; --i)
        {

            if (values[cols * i + j] == 0)
            {
                if (!found_free)
                    found_free = 1;
            }
            else
            {
                if (found_free)
                    return 0;
            }
        }
    }
    return 1;
}