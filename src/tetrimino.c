#include <ncurses.h>

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
    const int* values;
    const int cols;
    const int rows;
    const TetriminoType type;
} Tetrimino;

/* WORK IN PROGRESS */
struct Tetrimino getTetrimino(TetriminoType type)
{
    Tetrimino t;
    t.type = type;
    switch (type)
    {
        case T_I: {
            int values[1][4] = {
                    1, 1, 1, 1
            };
            t.values = &values[0][0];
            t.cols = 4;
            t.rows = 1;
            break;
        }
        case T_J: {
            int values[2][3] = {
                    2, 0, 0,
                    2, 2, 2,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_L: {
            size_t i,j;
            int values[2][3] = {
                    0, 0, 3,
                    3, 3, 3
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_S: {
            int values[2][3] = {
                    0, 5, 5,
                    5, 5, 0,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_Z: {
            int values[2][3] = {
                    7, 7, 0,
                    0, 7, 7,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_O: {
            int values[2][2] = {
                    4, 4,
                    4, 4,
            };

            t.values = &values[0][0];
            t.cols = 2;
            t.rows = 2;
            break;
        }
        case T_T: {
            int values[2][3] = {
                    0, 6, 0,
                    6, 6, 6,
            };

            t.values = &values[0][0];
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
 * Struttura che associa ad ogni tetramino
 * la sua quantità rimanente
 */
typedef struct TetriminoSet
{
    enum TetriminoType tetrimino;
    size_t remaining;
} TetriminoSet;