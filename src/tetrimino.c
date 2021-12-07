#include <ncurses.h>
#include <malloc.h>
#include <tetrimino.h>

#define N_tetrimini 7
#define TETRIMINOS_PER_TYPE 20

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

    T_I_90 = 7,
    T_J_90 = 8,
    T_L_90 = 9,
    T_S_90 = 10,
    T_O_90 = 11,
    T_Z_90 = 12,
    T_T_90 = 13,

    T_I_180 = 14,
    T_J_180 = 15,
    T_L_180 = 16,
    T_S_180 = 17,
    T_O_180 = 18,
    T_Z_180 = 19,
    T_T_180 = 20,

    T_I_270 = 21,
    T_J_270 = 22,
    T_L_270 = 23,
    T_S_270 = 24,
    T_O_270 = 25,
    T_Z_270 = 26,
    T_T_270 = 27,
} tetrimino_type_t;

/**
 * @brief In caso serva fare un loop di tutti i tipi
 * @deprecated Basta fare un loop da 0 a 6, gli enum si comportano come interi
 */
const tetrimino_type_t ALL_T_TYPES[N_tetrimini] = { T_I, T_J, T_L, T_O, T_S, T_T, T_Z };

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
 * @return Il tetramino.
 */
tetrimino_t *getTetrimino(int type)
{
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
        case T_I_90:
            t->values = malloc(sizeof(int)*1*4);
            t->values[0] = 1;
            t->values[1] = 1;
            t->values[2] = 1;
            t->values[3] = 1;

            t->cols = 1;
            t->rows = 4;
            break;
        case T_I_180:
            t = getTetrimino(T_I);
            break;
        case T_I_270:
            t = getTetrimino(T_I_90);
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
        case T_J_90:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 2;
            t->values[1] = 2;
            t->values[2] = 2;
            t->values[3] = 0;
            t->values[4] = 2;
            t->values[5] = 0;

            t->cols = 2;
            t->rows = 3;
            break;
        case T_J_180:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 2;
            t->values[1] = 2;
            t->values[2] = 2;
            t->values[3] = 0;
            t->values[4] = 0;
            t->values[5] = 2;

            t->cols = 3;
            t->rows = 2;
            break;
        case T_J_270:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 0;
            t->values[1] = 2;
            t->values[2] = 0;
            t->values[3] = 2;
            t->values[4] = 2;
            t->values[5] = 2;

            t->cols = 2;
            t->rows = 3;
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
        case T_L_90:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 3;
            t->values[1] = 0;
            t->values[2] = 3;
            t->values[3] = 0;
            t->values[4] = 3;
            t->values[5] = 3;

            t->cols = 2;
            t->rows = 3;
            break;
        case T_L_180:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 3;
            t->values[1] = 3;
            t->values[2] = 3;
            t->values[3] = 3;
            t->values[4] = 0;
            t->values[5] = 0;

            t->cols = 3;
            t->rows = 2;
            break;
        case T_L_270:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 3;
            t->values[1] = 3;
            t->values[2] = 0;
            t->values[3] = 3;
            t->values[4] = 0;
            t->values[5] = 3;

            t->cols = 2;
            t->rows = 3;
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
        case T_S_90:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 5;
            t->values[1] = 0;
            t->values[2] = 5;
            t->values[3] = 5;
            t->values[4] = 0;
            t->values[5] = 5;

            t->cols = 2;
            t->rows = 3;
            break;
            case T_S_180:
                t = getTetrimino(T_S);
            break;
            case T_S_270:
                t = getTetrimino(T_S_90);
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
        case T_O_90:
            t = getTetrimino(T_O);
        case T_O_180:
            t = getTetrimino(T_O);
        case T_O_270:
            t = getTetrimino(T_O);

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
        case T_Z_90:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 0;
            t->values[1] = 7;
            t->values[2] = 7;
            t->values[3] = 7;
            t->values[4] = 7;
            t->values[5] = 0;

            t->cols = 2;
            t->rows = 3;
            break;
        case T_Z_180:
            t = getTetrimino(T_Z);
        case T_Z_270:
            t = getTetrimino(T_Z_90);

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
        case T_T_90:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 6;
            t->values[1] = 0;
            t->values[2] = 6;
            t->values[3] = 6;
            t->values[4] = 6;
            t->values[5] = 0;

            t->cols = 2;
            t->rows = 3;
            break;
        case T_T_180:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 6;
            t->values[1] = 6;
            t->values[2] = 6;
            t->values[3] = 0;
            t->values[4] = 6;
            t->values[5] = 0;

            t->cols = 3;
            t->rows = 2;
            break;
        case T_T_270:
            t->values = malloc(sizeof(int)*2*3);
            t->values[0] = 0;
            t->values[1] = 6;
            t->values[2] = 6;
            t->values[3] = 6;
            t->values[4] = 0;
            t->values[5] = 6;

            t->cols = 2;
            t->rows = 3;
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
 * @brief La struct della finestra con tutti i pezzi dei tetramini rimanenti
 */
typedef struct TetriminiPool
{
    WINDOW *win;
    int *rem_tetriminos;    
} tetrimini_pool_t;


/**
 * @brief funzione per inizializzare la pool dove sono presenti tutti i tetramini rimasti
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
 * @brief Funzione STUB che stampa i tetramini in modalitá "menu"
 * @param[in] i l'indice del menu
 * @param[in] pool la pool nella quale stampare
 */
void printMenuStyle(int i, tetrimini_pool_t *pool){
        tetrimino_t *t = getTetrimino(i);
        printTetrimino(pool->win, t, 2+(i*3),3);
        free_tetrimino(t);
        mvwprintw(pool->win, getcury(pool->win), POOL_COLS / 2 ,"Rimanenti:%10d", pool->rem_tetriminos[i]);
}

/**
 * @brief permette di accedere al metodo di selezione dei tetramini
 * @param [in] w Finestra della pool da cui selezionare il pezzo
 * @return il numero della codifica del tetramino
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

    return i;
}

tetrimino_t* rotateTetrimino(tetrimino_t *t){
    if(get_tet_type(t)<((N_tetrimini*4)-1)){
        return t = getTetrimino(get_tet_type(t)+N_tetrimini);   
    }
    return t = getTetrimino(get_tet_type(t)%4);
}

#pragma region GETTERS

/**
 * @brief restituisce la finestra della pool di tetramini
 * @param[in] t la pool di tetramini
 */

WINDOW *getPoolWin(tetrimini_pool_t *t){
    return t->win;
}

int get_tet_rows(tetrimino_t *t)
{
    return t->rows;
}

int get_tet_cols(tetrimino_t *t)
{
    return t->cols;
}

int *get_tet_values(tetrimino_t *t)
{
    return t->values;
}

int get_tet_type(tetrimino_t *t){
    return t->type;
}

#pragma endregion