#include <malloc.h>

#include "../include/tetrimino.h"
#include "../include/gamefield.h"
#include "../include/constants.h"
#include "../include/bot.h"
#include "../include/functions.h"

/*******************************************************************************************
 *                                  IT'S BRUTEFORCE TIME!!!                                *
 * Prova tutte le board con tutti i tetramini e tutte le rotazioni                         *
 * si salva X punteggi piú alti (dove X varia a seconda della difficoltá imopostata)       *
 *                                                                                         *
 * In dettaglio:                                                                           *
 * - Copio la board di partenza e provo una combinazione, vedo il suo punteggio in base a: *
 * - blocchi occupati,                                                                     *
 * - righe completate,                                                                     *
 * - se fa finire il gioco per riempimento,                                                *
 * - confronto con il campo da gioco precedente.                                           *
 *******************************************************************************************/

/**
 * @brief struct della strategia di gioco, contentiene il suo campo da gioco e il "punteggio" della strategia.
 */
typedef struct Strategy
{
    int score;
    int tet;
    int rotation;
    int cursor;
    int *field;
} strategy_t;

/**
 * @brief funzione che istanzia la strategia DEEP-copiando giá la board.
 *
 * @param[in] field il puntatore al campo da gioco del bot
 *
 * @return puntatore alla strategia
 */
strategy_t *strategy_create(int *field)
{
    strategy_t *strategy = (strategy_t *)malloc(sizeof(strategy_t));
    strategy->score = 0;
    strategy->field = clone_field(field);
    return strategy;
}

/**
 * @brief distrugge la strategia puntata dal parametro.
 *
 * @param[in] strategy il puntatore alla strategia.
 */
void strategy_destroy(strategy_t *strategy)
{
    free(strategy->field);
    free(strategy);
}

/**
 * @brief funzione che ritorna il cursore della strategia passata come parametro.
 *
 * @param[in] s il puntatore alla strategia.
 * 
 * @return il cursore sul campo.
 */
int get_strategy_cursor(strategy_t *s)
{
    return s->cursor;
}

/**
 * @brief funzione che ritorna il tipo di tetramino usato dalla strategia passata come parametro.
 *
 * @param[in] s il puntatore alla strategia.
 * 
 * @return il tipo di tetramino usato dalla strategia.
 */
int get_strategy_tet_type(strategy_t *s)
{
    return s->tet;
}

/**
 * @brief funzione che ritorna la rotazione del tetramino usato della strategia passata come parametro.
 *
 * @param[in] s il puntatore alla strategia.
 * 
 * @return la rotazione del tetramino.
 */
int get_strategy_tet_rotation(strategy_t *s)
{
    return s->rotation;
}

/**
 * @brief popola tutti i campi della struct strategia con i valori passati come parametri
 * e calcola quelli assenti come lo score.
 *
 * @param[in] strategy il puntatore alla strategia.
 * @param[in] tetrimino it puntatoro al tetramino usato.
 * @param[in] cur_pos la posizione del cursore.
 * @param[in] rotation la rotazione del tetramino.
 * @param[in] last_used_tetrimino il tetramino usato precedentemente.
 * @param[in] last_cursor il cursore usato precedentemente.
 */
void strategy_update(strategy_t *strategy, tetrimino_t *tetrimino, int cur_pos, int rotation, int last_used_tetrimino, int last_cursor)
{

    int *tmp = clone_field(strategy->field);
    tetrimino_t *ttemp = get_tetrimino(last_used_tetrimino);

    /* aggiunge il tetramino alla board */
    int drops = add_tetrimino_to_field(strategy->field, tetrimino, cur_pos);

    /* salva il cursore */
    strategy->cursor = cur_pos;

    /* salva la rotazione del tetramino */
    if (rotation != 4)
        strategy->rotation = rotation;

    /* calcola il punteggio */
    strategy->score = calculate_score(strategy, tmp, tetrimino, ttemp, drops, last_cursor);

    /* salva il tipo di tetramino */
    strategy->tet = get_tet_type(tetrimino);

    /* distrugge la board temporanea copiata dalla vecchia e il tetramino*/
    free(tmp);
    free_tetrimino(ttemp);
}

/**
 * @brief Funzione **ricorsiva** che compara la strategia passata per parametro con le migliori.
 * Se ci sono slot liberi, la inserisce automaticamente nel primo di questi.
 * Se invece non ci sono slot liberi, constrolla le altre strategie e se è migliore
 * di una di queste la sostituisce.
 *
 * @param[in] best L'array di strategie.
 * @param[in] size La dimensione dell'array.
 * @param[in] str La strategia da inserire.
 *
 * @return 1 se ha inserito la strategia nell'array, 0 altrimenti.
 */
int set_strategy(strategy_t **best, int size, strategy_t *str)
{
    if (size <= 0)
        return 0;
    else if (best[0] == NULL)
    {
        best[0] = str;
        return 1;
    }
    else
    {
        if (set_strategy(best + 1, size - 1, str))
            return 1;
        else if (best[0]->score <= str->score)
        {
            strategy_destroy(best[0]);
            best[0] = str;
            return 1;
        }
        else
            return 0;
    }
}

/**
 * @brief Funzione di bruteforce del bot.
 * @param[in] g Il campo su cui preparare una strategia.
 * @param[in] pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @param[in] err La variabile che indica quante strategie il bot deve prendere in considerazione.
 *
 * @return La strategia che il bot considera migliore.
 */
strategy_t *choose_strategy(gamefield_t *g, tetrimini_pool_t *pool, int err)
{

    int last_used_tet = -1;
    int last_cur = -1;

    strategy_t **best_strategies = (strategy_t **)calloc(sizeof(strategy_t *), err);

    int i, j, k, l, m = 0;

    int chosen = rand() % err;

    for (i = 0; i < N_tetrimini; i++)
    {
        if (get_remaining_tetriminos(pool, i) > 0)
        {
            tetrimino_t *t = get_tetrimino(i);

            linear_rotate(t, 1);
            for (j = 0; j < FIELD_COLS; j++)
            {

                for (k = 0; k < 4; k++)
                {
                    strategy_t *str = strategy_create(get_gamefield(g));

                    if (!safe_rotate_tetrimino(t, j, 0))
                    {
                        linear_rotate(t, 0);
                        strategy_destroy(str);
                        continue;
                    }
                    ++m;

                    /*faccio finalmente la strategia*/
                    strategy_update(str, t, j, k, last_used_tet, last_cur);
                    if (!set_strategy(best_strategies, err, str))
                    {
                        strategy_destroy(str);
                    }
                }
            }
            free_tetrimino(t);
        }
    }

    /*ho le migliori X strategie, ne ritorno una random*/
    strategy_t *tmp = best_strategies[chosen];

    last_used_tet = tmp->tet;
    last_cur = tmp->cursor;

    for (i = 0; i < err; ++i)
    {
        if (i != chosen)
            strategy_destroy(best_strategies[i]);
    }
    free(best_strategies);

    return tmp;
}

/**
 * @brief Funzione che calcola la strategia migliore del bot.
 *
 * @param[in] s il puntatore alla strategia
 * @param[in] old il puntatore al campo da gioco precendente alla mossa
 * @param[in] tet il puntatore al tetramino usato dalla mossa
 * @param[in] last_used_tet il puntatore al tetramino usato dalla mossa precedente
 * @param[in] last_cursor il cursore usato dalla mossa precedente
 * @param[in] drops flag per sapere se é possibile effettuare il drop (se quella mossa fa finire la partita)
 *
 * @return Il punteggio della strategia.
 */
int calculate_score(strategy_t *s, int *old, tetrimino_t *tet, tetrimino_t *last_used_tet, int last_cursor, int drops)
{

    /* parto da num_blocchi e tolgo punti se la riga non é piena ma ho aggiunto blocchi */
    int score = FIELD_ROWS * FIELD_COLS;
    int i, j, cur_diff, min = 0;

    /* se fa finire il gioco semplicemente fa schifo come opzione */
    if (!drops)
    {
        return -FIELD_COLS*FIELD_ROWS*1000; /* immagina ricevere -1000 punti LMAO */
    }

    cur_diff = last_cursor - s->cursor;
    if (cur_diff < 0)
    {
        cur_diff = cur_diff * -1;
    }

    if (last_cursor == s->cursor)
    {
        score -= 1000;
    }
    else if (cur_diff < FIELD_COLS / 4)
    {
        score -= 800;
    }

    if (get_tet_type(tet) == get_tet_type(last_used_tet))
    {
        score -= 650;
    }

    if (are_tet_equal(tet, last_used_tet))
    {
        score -= 850;
    }

    for (i = 0; i < FIELD_COLS; i++)
    {
        int fr = get_first_free_row_in_field(old, i);
        if (fr > min)
        {
            min = fr;
        }
    }
    if (s->cursor == min)
    {
        score += 500;
    }
    else
    {
        score -= 1000;
    }

    for (i = 0; i < FIELD_ROWS; i++)
    {
        if (!is_row_full(s->field, i))
        {
            for (j = 0; j < FIELD_COLS; j++)
            {
                if (s->field[i * FIELD_COLS + j] != 0)
                {
                    score--;
                }
            }
        }
        else
        {
            score += FIELD_COLS * 10; /* se la riga é piena, aggiungo punti (FIELD_COLS + un bonus) */
        }
    }

    return score - blank_cells(s->field, FIELD_ROWS, FIELD_COLS) * 100;
}