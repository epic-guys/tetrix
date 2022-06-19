#include <malloc.h>

#include "../include/tetrimino.h"
#include "../include/gamefield.h"
#include "../include/constants.h"
#include "../include/bot.h"
#include "../include/functions.h"

/**
 * IT'S BRUTEFORCE TIME!!!
 * Prova tutte le board con tutti i tetramini e tutte le rotazioni, si salva i tre punteggi piú alti :)
 * In dettaglio:
 * Copio la board di partenza e provo una combinazione, vedo il suo punteggio in base a:
 * blocchi occupati,
 * righe completate,
 * se fa finire il gioco per riempimento.
 */
 /**
  * @brief struct della strategia di gioco, contentiene il suo campo da gioco e il "punteggio" della strategia.
  */
typedef struct Strategy
{
    int score;
    int tet;
    int rotation;
    int cursor;
    int* field;
} strategy_t;

/**
 * @brief funzione che istanzia la strategia copiando giá la board
 *
 * @param[in] field
 * @return puntatore alla strategia
 */
strategy_t* strategy_create(int* field)
{
    strategy_t* strategy = (strategy_t*)malloc(sizeof(strategy_t));
    strategy->score = 0;
    strategy->field = clone_field(field);
    return strategy;
}

void strategy_destroy(strategy_t* strategy)
{
    free(strategy->field);
    free(strategy);
}

int get_strategy_cursor(strategy_t* s)
{
    return s->cursor;
}

int get_strategy_tet_type(strategy_t* s)
{
    return s->tet;
}

int get_strategy_tet_rotation(strategy_t* s)
{
    return s->rotation;
}

void strategy_update(strategy_t* strategy, tetrimino_t* tetrimino, int cur_pos, int rotation, int last_used_tetrimino)
{
    int* tmp = clone_field(strategy->field);

    //aggiunge il tetramino alla board
    add_tetrimino_to_field(strategy->field, tetrimino, cur_pos);

    //salva il cursore
    strategy->cursor = cur_pos;

    //salva la rotazione del tetramino
    strategy->rotation = rotation;

    //calcola il punteggio
    strategy->score = calculate_score(strategy->field, tmp, get_tet_type(tetrimino), last_used_tetrimino);

    strategy->tet = get_tet_type(tetrimino);

    //free_tetrimino(tetrimino);
    free(tmp);
}

/**
 * @brief Funzione **ricorsiva** che compara la strategia passata per parametro con le migliori.
 * Se ci sono slot liberi, la inserisce automaticamente nel primo di questi.
 * Se invece non ci sono slot liberi, constrolla le altre strategie e se è migliore
 * di una di queste la sostituisce.
 *
 * @param best L'array di strategie.
 * @param size La dimensione dell'array.
 * @param str La strategia da inserire.
 * @return 1 se ha inserito la strategia nell'array, 0 altrimenti.
 */
int set_strategy(strategy_t** best, int size, strategy_t* str)
{
    if (size <= 0)
        return 0;
    else if (best[0] == NULL) {
        best[0] = str;
        return 1;
    }
    else
    {
        if (set_strategy(best + 1, size - 1, str))
            return 1;
        else if (best[0]->score < str->score)
        {
            strategy_destroy(best[0]);
            best[0] = str;
            return 1;
        }
        else return 0;
    }
}

/**
 * @brief Funzione di bruteforce del bot.
 * @param[in] g Il campo su cui preparare una strategia.
 * @param[in] pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @return La strategia che il bot considera migliore.
 */
strategy_t* choose_strategy(gamefield_t* g, tetrimini_pool_t* pool,int err)
{
    strategy_t** best_strategies = (strategy_t**) calloc(sizeof(strategy_t*), err); 
    
    int i, j, k, l, last_used_tet = -1;
    srand(time(NULL));
    int choosen = rand() % 3;

    for (i = 0; i < N_tetrimini; i++) {
        if (0 == get_remaining_tetriminos(pool, i))
        {
            continue;
        }

        tetrimino_t* t = get_tetrimino(i);
        int cols = FIELD_COLS - get_tet_cols(t);
        for (j = 0;j < cols;j++) {

            for (k = 0;k < 4;k++) {

                strategy_t* str = strategy_create(get_gamefield(g));

                if (!safe_rotate_tetrimino(t, j, 0)) {
                    strategy_destroy(str);
                    continue;
                }

                //cols = (FIELD_COLS-get_tet_cols(t))/2;
                /*faccio finalmente la strategia*/
                strategy_update(str, t, j, k, last_used_tet);

                if (!set_strategy(best_strategies, err, str)) {
                    strategy_destroy(str);
                }

            }

        }
    }

    /*ho le migliori X strategie, ne ritorno una random*/
    strategy_t* tmp = best_strategies[choosen];
    last_used_tet = tmp->tet;

    for (i = 0; i < err; ++i)
    {
        if (i != choosen)
            strategy_destroy(best_strategies[i]);
        best_strategies[i] = NULL;
    }
    free(best_strategies);
    return tmp;
}

int calculate_score(int* field, int* old, int tet_type, int last_used_tet)
{
    /**
     * TUTORIAL: come si calcola lo score
     * chi ha meno blocchi occupati,
     * righe completate,
     * se fa finire il gioco per riempimento.
     */

     //parto da num_blocchi e tolgo punti se la riga non é piena ma ho aggiunto blocchi
    int score = FIELD_ROWS * FIELD_COLS;
    int i, j;
    //se fa finire il gioco semplicemente fa schifo come opzione
    if (is_field_top_occupied(field)) {
        return 0; //immagina ricevere zero punti LMAO
    }

    if (last_used_tet == tet_type) {
        score -= 30;
    }



    for (i = 0; i < FIELD_ROWS / 2; i++) {
        for (j = 0; j < FIELD_COLS / 2; j++) {
            if (!is_row_full(field, i)) {
                if (field[i * FIELD_COLS / 2 + j] != 0) {
                    score--;
                }
            }
            else {
                score += FIELD_COLS / 2 + 10; //se la riga é piena, aggiungo punti (FIELD_COLS + un bonus)
            }
        }
    }
    return score - compare_fields(field, old);
}

int compare_fields(int* new, int* old)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS / 2; i++) {
        for (j = 0; j < FIELD_COLS / 2; j++) {
            if (new[i * FIELD_COLS / 2 + j] != old[i * FIELD_COLS / 2 + j]) {
                return i * 4;
            }
        }
    }
    return FIELD_ROWS / 2;
}