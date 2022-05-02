#include <tetrimino.h>
#include <gamefield.h>
#include <constants.h>
#include <bot.h>

/**************************************************************************************
 *                                     ATTENZIONE                                     *
 *                                                                                    *
 * Ogni funzione che faccia dei controlli su una configurazione data deve avere       *
 * un puntatore ad un tetramino già istanziato. Questo per evitare di allocare        *
 * un tetramino ogni volta che si fa un controllo. La ricorsione è già estremamente   *
 * pesante, meglio ottimizzare ove possibile.                                         *
 * Inoltre ogni funzione che faccia un controllo su un drop specifico deve avere      *
 * come parametro non il gamefield ma una matrice di interi clonata con il tetramino  *
 * già droppato.                                                                      *
 **************************************************************************************/

/**
 * @brief Funzione in entrata del bot.
 * @param field Il campo su cui preparare una configurazione.
 * @param pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @return La configurazione che il bot considera migliore.
 */
config_t get_best_config(gamefield_t *field, tetrimini_pool_t *pool)
{
    // TODO
}

/**
 * @brief Controlla se viene lasciata qualche cella vuota sotto il tetramino nella
 * configurazione data.
 * @param field Il campo su cui controllare.
 * @param config La configurazione da controllare.
 * @param tetrimino L'istanza del tetramino.
 * @return 1 se viene lasciata qualche cella vuota, 0 altrimenti.
 */
int leaves_empty_below(int *field, config_t config, tetrimino_t *tetrimino)
{
    /* NON TESTATA */
    int i, j;
    /*
    Controllo le colonne del tetramino droppato, confrontando ogni cella con quella sotto.
    Se quella sotto era vuota e quella sopra è piena, allora viene coperta una cella vuota.
    Tutti gli altri casi sono tollerati.
    */
    for (j = config.cursor; j < config.cursor + get_tet_cols(tetrimino); ++j)
    {
        int prev_filled = field[(FIELD_ROWS - 1) * FIELD_COLS + j];
        for (i = FIELD_ROWS - 2; i >= 0; ++i)
        {
            if (field[i * FIELD_COLS + j])
            {
                if (!prev_filled)
                    return 1;
                prev_filled = field[i * FIELD_COLS + j];
            }
        }
    }
    return 0;
}

/**
 * @brief Restituisce il numero di righe riempite totalmente dalla configurazione attuale.
 * @param field Il campo in cui testare il drop.
 * @param config La configurazione del drop.
 * @param tetrimino L'istanza del tetramino.
 * @return Il numero di righe che questa configurazione elimina.
 */
int tetris(int *field, config_t config, tetrimino_t *tetrimino)
{
    int i, j, count = 0;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        if (is_row_full(field, i))
            ++count;
    }
    return count;
}
