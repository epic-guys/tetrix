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
enum TetriminoType
{
    T_I,
    T_J,
    T_L,
    T_S,
    T_O,
    T_Z,
    T_T
}

const allTypes[7] = {T_I,T_J,T_L,T_O,T_S,T_T,T_Z};

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
    enum TetriminoType type;
} Tetrimino;

/**
 * Per gestire la rotazione si potrebbe
 * usare un int, ma con l'enum si limitano
 * le rotazioni possibili a questi 4 angoli
 * precisi
 * La D iniziale sta per "degrees"
*/
enum Rotation
{
    D_0,
    D_90,
    D_180,
    D_270
};

/**
 * Struttura che associa ad ogni tetramino
 * la sua quantità rimanente
 */
typedef struct TetriminoSet
{
    enum TetriminoType tetrimino;
    size_t remaining;
} TetriminoSet;