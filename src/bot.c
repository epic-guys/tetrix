#include "../include/tetrimino.h"
#include "../include/gamefield.h"
#include "../include/constants.h"
#include "../include/bot.h"

/**
 * IT'S BRUTEFORCE TIME!!!
 * Prova tutte le board con tutti i tetramini e tutte le rotazioni, si salva i tre punteggi piú alti :)
 * In dettaglio:
 * Copio la board di partenza e provo una combinazione, vedo il suo punteggio in base a: 
 * blocchi occupati,
 * righe completate,
 * se fa finire il gioco per riempimento.
 */

typedef struct strategy{
    int score;
    int *field;
} strategy_t;

/**
 * @brief funzione che istanzia la strategia copiando giá la board
 * 
 * @param field 
 * @return strategy_t* 
 */
strategy_t *strategy_create(int *field){
    strategy_t *strategy = malloc(sizeof(strategy_t));
    strategy->score = 0;
    strategy->field = (int*) malloc(sizeof(int) * (FIELD_COLS * FIELD_ROWS));
    // copio il field
    memcpy(strategy->field, field, sizeof(int) * (FIELD_COLS * FIELD_ROWS));
    return strategy;
}

void *strategy_destroy(strategy_t *strategy){
    free(strategy->field);
    free(strategy);
}

int calculate_score(int *field){
/**
 * TUTORIAL: come si calcola lo score
 * chi ha meno blocchi occupati,
 * righe completate,
 * se fa finire il gioco per riempimento.
 */

    //parto da num_blocchi e tolgo punti se la riga non é piena ma ho aggiunto blocchi
    int score = FIELD_ROWS * FIELD_COLS;
    int i, j;
    if(is_gamefield_top_occupied(field)){
        score = 0;
    }
    for(i = 0; i < FIELD_ROWS; i++){
        for(j = 0; j < FIELD_COLS; j++){
            if(!is_row_full(field, i)){
                if(field[i * FIELD_COLS + j] != 0){
                    score--;
                }
            }
            else{
                score+=FIELD_COLS+10; //se la riga é piena, aggiungo punti (FIELD_COLS + un bonus)
            }
        }
    }
    return score;
}

void strategy_update(strategy_t *strategy, tetrimino_t *tetrimino, int cur_pos){
    
    //piazza il tetrmino nella board
    add_tetrimino_to_gamefield(strategy->field, tetrimino, cur_pos);
    //calcola il punteggio
    strategy->score = calculate_score(strategy->field);
}

/**
 * @brief Funzione in entrata del bot.
 * @param field Il campo su cui preparare una configurazione.
 * @param pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @return La configurazione che il bot considera migliore.
 */
strategy_t choose_strategy(gamefield_t *field, tetrimini_pool_t *pool)
{
    int i;
    for(i=0;i<N_tetrimini; i++){

    }
}
