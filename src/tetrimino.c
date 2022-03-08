#include <ncurses.h>
#include <malloc.h>

#include <tetrimino.h>
#include <gamefield.h>
#include <constants.h>


/**
 * @brief Codifica di ogni tetramino.
 * 
 * T_I:
 * ████████
 * T_J:
 *   ██
 *   ██
 * ████
 * T_L:
 * ██
 * ██
 * ████
 * T_S:
 *   ████
 * ████
 * T_O:
 * ████
 * ████
 * T_Z:
 * ████
 *   ████
 * T_T:
 * ██████
 *   ██
 */
typedef enum tetrimino_type
{
    T_I = 0,
    T_J = 1,
    T_L = 2,
    T_S = 3,
    T_O = 4,
    T_Z = 5,
    T_T = 6,
} tetrimino_type_t;

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
    int* values;
    int cols;
    int rows;
    tetrimino_type_t type;
} tetrimino_t;

/**
 * @brief Genera il tetramino dato il suo enum. 
 * 
 * @param[in] type la sua codifica enum.
 * @param[out] Il tetramino.
 */
tetrimino_t *getTetrimino(int type){
    int *values;
    tetrimino_t *t = malloc(sizeof(tetrimino_t));
    t->type = type;
    switch (type)
    {
        case T_I:
            t->values = malloc(sizeof(int)*1*4);
            t->values[0] = 1;
            t->values[1] = 1;
            t->values[2] = 1;
            t->values[3] = 1;

            t->cols = 4;
            t->rows = 1;
            break;
        
        case T_J:
            t->values = malloc(sizeof(int)*2*3);
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
            t->values = malloc(sizeof(int)*2*3);
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
            t->values = malloc(sizeof(int)*2*3);
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
            t->values = malloc(sizeof(int)*2*2);
            t->values[0] = 4;
            t->values[1] = 4;
            t->values[2] = 4;
            t->values[3] = 4;

            t->cols = 2;
            t->rows = 2;
            break;

        case T_Z:
            t->values = malloc(sizeof(int)*2*3);
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
            t->values = malloc(sizeof(int)*2*3);
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
            printf("Non so come hai fatto ma hai passato un valore non presente nell'enum");
    }
    return t;
}

/**
 * @brief Libera la memoria allocata dal tetrimino.
 * 
 * @param[in, out] t Il tetrimino da deallocare.
 */
void freeTetrimino(tetrimino_t *t){
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
void printTetrimino(WINDOW *w,tetrimino_t *t,int y,int x){
    int i,j,c=0;
    for(i=0;i<t->rows;++i){
        for(j=0;j<t->cols;++j){
            if(t->values[c])
            {
                wattron(w, COLOR_PAIR(t->values[c]));
                mvwprintw(w,y+i,(x+j) * 2,"[]");
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
} tetrimini_pool_t;


/**
 * @brief funzione per inizializzare la pool dove sono presenti tutti i tetramini rimasti.
 * 
 * @param[in] y La riga da dalla quale deve iniziare a poizionare la finestra.
 * @param[in] x La colonna da dalla quale deve iniziare a poizionare la finestra.
 * @return Lo struct della pool istanziato.
 */
tetrimini_pool_t *initializePool(int y, int x){
    int i;
    WINDOW *w;
    tetrimini_pool_t *tetriminiPool = (tetrimini_pool_t*) malloc(sizeof(tetrimini_pool_t));
    tetriminiPool->rem_tetriminos = malloc(sizeof(int) * N_tetrimini);
    for(i = 0; i < N_tetrimini; ++i)
    {
        tetriminiPool->rem_tetriminos[i] = TETRIMINOS_PER_TYPE;
    }

    w = newwin(POOL_ROWS, POOL_COLS, y, x);
    box(w,0,0);
    wmove(w,getcurx(w)+1,getcury(w)+2);
    wprintw(w,"TETRAMINI DISPONIBILI: ");
    wrefresh(w);
    tetriminiPool->win = w;
    return tetriminiPool;
}

/**
 * @brief free della pool di tetramini.
 * 
 * @param[in] p La pool.
 */
void freePool(tetrimini_pool_t* p){
    free(p->rem_tetriminos);
    free(p);
}

/**
 * @brief Funzione STUB che stampa i tetramini in modalitá "menu".
 * 
 * @param[in] i l'indice del menu.
 * @param[in] pool la pool nella quale stampare.
 */
void printMenuStyle(int i, tetrimini_pool_t *pool){
        tetrimino_t *t = getTetrimino(i);
        printTetrimino(pool->win, t, 2+(i*3),3);
        freeTetrimino(t);
        mvwprintw(pool->win, getcury(pool->win), POOL_COLS / 2 ,"Rimanenti:%10d", pool->rem_tetriminos[i]);
}

/**
 * @brief permette di accedere al metodo di selezione dei tetramini.
 * 
 * @param [in] w Finestra della pool da cui selezionare il pezzo.
 * @param[out] n il numero della codifica del tetramino.
 */
int selectTetrimino(tetrimini_pool_t *pool){

    int i, ch;

    /* Stampa il menu iniziale*/
    for(i=0;i<N_tetrimini;++i) {
        if(i == 0)
            /*"sottolinea" il primo elemento*/
            wattron( pool->win, A_STANDOUT );
        else
            wattroff( pool->win, A_STANDOUT );
        printMenuStyle(i,pool);
    }

    /*carica lo schermo*/
    wrefresh(pool->win);
    i = 0;
    /*sposta il focus della tastiera sulla finestra*/
    keypad(pool->win,TRUE);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
    
    do{
        ch = wgetch(pool->win); 
        printMenuStyle(i,pool);
        switch(ch) {
            case KEY_UP:
                i--;
                i = (i<0) ? N_tetrimini-1 : i;
            break;
            case KEY_DOWN:
                i++;
                i = (i>N_tetrimini-1) ? 0 : i;
            break;
        }
        
        /*Sottolinea la scelta*/
        wattron( pool->win, A_STANDOUT );
        printMenuStyle(i,pool);
        wattroff( pool->win, A_STANDOUT );
        wrefresh( pool->win );
    
    }while(ch != '\n');
    if(pool->rem_tetriminos[i]>0){
        removeTetriminoFromPool(i,pool);
        return i;
    }
    return selectTetrimino(pool);
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
 */
void linearRotate(tetrimino_t *t){
    int *v_rotate = (int *)malloc(sizeof(int) * t->rows * t->cols);
    size_t i, j;

    for (i = 0; i < t->rows; ++i)
    {
        for (j = 0; j < t->cols; ++j)
        {
            int i_rot = j;
            int j_rot = -i + t->rows - 1;
            v_rotate[i_rot * t->rows + j_rot] = t->values[i * t->cols + j];
        }
    }
    free(t->values);
    t->values = v_rotate;
    i = t->rows;
    t->rows = t->cols;
    t->cols = i;
}

/**
 * @brief Stub di rotateTetrimino che verifica la fattibilitá della rotazione altrimenti non ruota.
 * 
 * @param[in] t Tetramino da ruotare.
 * @param[in] cur_pos posizione del cursore.
 */
void safeRotateTetrimino(tetrimino_t *t, int cur_pos){

    if (cur_pos + t->rows <= FIELD_COLS)
        linearRotate(t);
}

/**
 * @brief refresh della pool.
 * 
 * @param[in] p la pool.
 */
void refreshPool(tetrimini_pool_t *p){
    wrefresh(p->win);
    refresh();
}

/**
 * @brief rimuove un'unitá di un tetramino dalla pool.
 * 
 * @param[in] i il tipo di tetramino dalla quale togliere un'unitá.
 * @param[in] p la pool di tetramini.
 */
void removeTetriminoFromPool(int i, tetrimini_pool_t *p){
    if(p->rem_tetriminos[i]>0)
        p->rem_tetriminos[i]-=1;
    refreshPool(p);
}

/**
 * @brief controlla se ci sono tetramini rimanenti.
 * 
 * @param[in] pool la pool di tetramini selezionabili.
 * 
 * @param[out] i ritorna 0 se c'é almeno un tetramino di qualsiasi tipo, 1 altrimenti
 */
int noTetriminosLeft(tetrimini_pool_t *pool){
    int i;
    for(i=0;i<N_tetrimini;++i){
        if(pool->rem_tetriminos[i])
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
WINDOW *getPoolWin(tetrimini_pool_t *t){
    return t->win;
}

/**
 * @brief restituisce il numero di righe di un tetramino.
 * 
 * @param[in] t il tetramino.
 * @param[out] rows il numero di righe di cui é composto il tetramino.
 */
int getTetRows(tetrimino_t *t){
    return t->rows;
}

/**
 * @brief restituisce il numero di colonne di un tetramino.
 * 
 * @param[in] t il tetramino.
 * @param[out] cols il numero di colonne di cui é composto il tetramino.
 */
int getTetCols(tetrimino_t *t){
    return t->cols;
}

/**
 * @brief restituisce l'array di valori di un tetramino.
 * 
 * @param[in] t il tetramino.
 * @param[out] values il puntatore all'array di valori del tetramino.
 */
int *getTetValues(tetrimino_t *t){
    return t->values;
}

/**
 * @brief restituisce il tipo di tetramino.
 * 
 * @param[in] t il tetramino.
 * @param[out] type l'intero che identifica il tipo di tetramino.
 * 
 */
int getTetType(tetrimino_t *t){
    return t->type;
}
/**
 * @brief restituisce il colore del tetramino.
 * 
 * @param[in] t il tetramino.
 * @param[out] color il numero che identifica il colore del tetramino.
 */
int getTetColor(tetrimino_t *t){
    int i;
    int *values = getTetValues(t);
    for(i=0;i<(getTetCols(t)*getTetRows(t))-1;++i){
        if(values[i]!=0)
            return values[i];
    }
}