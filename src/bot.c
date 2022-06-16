#include <malloc.h>

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

typedef struct Strategy
{
    int score;
    int *field;
} strategy_t;

/**
 * @brief funzione che istanzia la strategia copiando giá la board
 * 
 * @param field 
 * @return strategy_t* 
 */
strategy_t *strategy_create(int *field)
{
    strategy_t *strategy = malloc(sizeof(strategy_t));
    strategy->score = 0;
    strategy->field = clone_field(field);
    return strategy;
}

void strategy_destroy(strategy_t *strategy)
{
    super_free(strategy->field);
    super_free(strategy);
}

void strategy_update(strategy_t *strategy, tetrimino_t *tetrimino, int cur_pos)
{
    //aggiunge il tetramino alla board
    add_tetrimino_to_field(strategy->field, tetrimino, cur_pos);
    //calcola il punteggio
    strategy->score = calculate_score(strategy->field);
}

/**
 * @brief Funzione di bruteforce del bot.
 * @param g Il campo su cui preparare una strategia.
 * @param pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @return La strategia che il bot considera migliore.
 */
void choose_strategy(gamefield_t *g, tetrimini_pool_t *pool)
{
    strategy_t* best_strategies[3] = { NULL, NULL, NULL };
    int i,j,k,l;
    int choosen = rand()%3;
    for(i = 0; i < N_tetrimini; i++){
        if(get_remaining_tetriminos(pool, i) == 0){
            break;
        }
        else{
            tetrimino_t *t = get_tetrimino(i);
            for(j=0;j<FIELD_COLS;j++){
                for(k=0;k<4;k++){
                    strategy_t *str = strategy_create(get_gamefield(g));
                    safe_rotate_tetrimino(t,j);
                    //faccio finalmente la strategia
                    strategy_update(str,t,j);

                    if(best_strategies[0] == NULL ){
                        best_strategies[0] = str;
                    }
                    else if(best_strategies[1] == NULL){
                        if(str->score > best_strategies[0]->score){
                            best_strategies[1] = best_strategies[0];
                            best_strategies[0] = str;
                        }else{ best_strategies[1] = str; }
                    } 
                    else if(best_strategies[2] == NULL){
                        best_strategies[2] = str;
                    }else{
                        //mvprintw(10,15,"else");
                        if(str->score > best_strategies[2]->score){
                            strategy_destroy(best_strategies[2]);
                            if(str->score > best_strategies[1]->score){
                                best_strategies[2] = best_strategies[1];
                                if(str->score > best_strategies[0]->score){
                                    best_strategies[1] = best_strategies[0];
                                    best_strategies[0] = str;
                                }else{ best_strategies[1] = str; }
                            }else{ best_strategies[2] = str; }
                        }else{ strategy_destroy(str); }
                    }
                }
            }
            //super_free_tetrimino(t);
        }
    }
    //ho le migliori tre strategie, ne ritorno una random
    for(i=0;i<3;i++){
        if(i != choosen){
            //strategy_destroy(best_strategies[i]);
        }
    }
    //set_field(g,best_strategies[choosen]->field);
    //set_field(g,best_strategies[0]->field);  
}

int calculate_score(int *field)
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
    if(is_field_top_occupied(field)){
        return 0; //immagina ricevere zero punti LMAO
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
