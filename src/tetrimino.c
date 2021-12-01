#include <ncurses.h>
#include <malloc.h>

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
typedef enum TetriminoType
{
    T_I,
    T_J,
    T_L,
    T_S,
    T_O,
    T_Z,
    T_T
} TetriminoType;

/* In caso serva fare un loop di tutti i tipi */
const TetriminoType ALL_T_TYPES[7] = { T_I, T_J, T_L, T_O, T_S, T_T, T_Z };

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
 */
typedef struct Tetrimino
{
    int* values;
    int cols;
    int rows;
    TetriminoType type;
} Tetrimino;

/**
 * @brief Genera il tetramino dato il suo enum. 
 * 
 * @param[in] type la sua codifica enum.
 * @return Il tetramino.
 */
Tetrimino getTetrimino(TetriminoType type)
{
    Tetrimino t;
    int *values;
    switch (type)
    {
        case T_I:
            t.values = malloc(sizeof(int) * 1 * 4);
            t.values[0] = 1;
            t.values[1] = 1;
            t.values[2] = 1;
            t.values[3] = 1;

            t.cols = 4;
            t.rows = 1;
            break;

        case T_J:
            t.values = malloc(sizeof(int) * 2 * 3);
            t.values[0] = 2;
            t.values[1] = 0;
            t.values[2] = 0;
            t.values[3] = 2;
            t.values[4] = 2;
            t.values[5] = 2;

            t.cols = 3;
            t.rows = 2;
            break;

        case T_L:
            t.values = malloc(sizeof(int) * 2 * 3);
            t.values[0] = 0;
            t.values[1] = 0;
            t.values[2] = 3;
            t.values[3] = 3;
            t.values[4] = 3;
            t.values[5] = 3;

            t.cols = 3;
            t.rows = 2;
            break;

        case T_S:
            t.values = malloc(sizeof(int) * 2 * 3);
            t.values[0] = 0;
            t.values[1] = 5;
            t.values[2] = 5;
            t.values[3] = 5;
            t.values[4] = 5;
            t.values[5] = 0;

            t.cols = 3;
            t.rows = 2;
            break;

        case T_Z:
            t.values = malloc(sizeof(int) * 2 * 3);
            t.values[0] = 7;
            t.values[1] = 7;
            t.values[2] = 0;
            t.values[3] = 0;
            t.values[4] = 7;
            t.values[5] = 7;

            t.cols = 3;
            t.rows = 2;
            break;

        case T_O:
            t.values = malloc(sizeof(int) * 2 * 2);
            t.values[0] = 4;
            t.values[1] = 4;
            t.values[2] = 4;
            t.values[3] = 4;

            t.cols = 2;
            t.rows = 2;
            break;

        case T_T: {
            t.values = malloc(sizeof(int) * 2 * 3);
            t.values[0] = 0;
            t.values[1] = 6;
            t.values[2] = 0;
            t.values[3] = 6;
            t.values[4] = 6;
            t.values[5] = 6;

            t.cols = 3;
            t.rows = 2;
            break;
        }
        default:
            printf("Non so come hai fatto ma hai passato un valore non presente nell'enum");
    }
    return t;
}

/**
 * @brief Struttura che associa ad ogni tetramino
 * la sua quantità rimanente
 */
typedef struct TetriminoSet
{
    TetriminoType tetrimino;
    size_t remaining;
} TetriminoSet;

/**
 * @brief La struct della finestra con tutti i pezzi dei tetramini rimanenti
 */
typedef struct TetriminiPool
{
    WINDOW *pool;
} TetriminiPool;


/**
 * @brief funzione per inizializzare la pool dove sono presenti tutti i tetramini rimasti
 */
TetriminiPool *initializePool(int x, int y){
    TetriminiPool *tetriminiPool = (TetriminiPool*) malloc(sizeof(TetriminiPool));
    WINDOW *w;

    w = newwin(20, 50, x, y);
    box(w,0,0);
    wmove(w,getcurx(w)+1,getcury(w)+2);
    wprintw(w,"TETRAMINI DISPONIBILI: ");
    wrefresh(w);
    tetriminiPool->pool = w;
    return tetriminiPool;
}

WINDOW *getPoolWin(TetriminiPool *t){
    return t->pool;
}

int selectTetrimino(WINDOW *w){
    /*TODO: scrivi tutto*/
}