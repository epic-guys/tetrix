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
    int *field;
} strategy_t;

/**
 * @brief funzione che istanzia la strategia copiando giá la board
 * 
 * @param[in] field 
 * @return puntatore alla strategia 
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

void strategy_update(strategy_t *strategy, tetrimino_t *tetrimino, int cur_pos,int last_used_tetrimino)
{   
    //aggiunge il tetramino alla board
    add_tetrimino_to_field(strategy->field, tetrimino, cur_pos);
   
    //calcola il punteggio
    strategy->score = calculate_score(strategy->field,tetrimino,last_used_tetrimino);
}

/**
 * @brief Funzione di bruteforce del bot.
 * @param[in] g Il campo su cui preparare una strategia.
 * @param[in] pool La pool di tetramini, per far sì che possa scegliere un tetramino rimanente.
 * @return La strategia che il bot considera migliore.
 */
void choose_strategy(gamefield_t *g, tetrimini_pool_t *pool)
{
    strategy_t* best_strategies[3] = { NULL, NULL, NULL };
    int i,j,k,l,last_used_tet=99;
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
                    
                    safe_rotate_tetrimino(t, j, 0);
                    
                    //faccio finalmente la strategia
                    strategy_update(str,t,j,last_used_tet);

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

                        if(str->score > best_strategies[0]->score){

                            best_strategies[2] = best_strategies[1];
                            best_strategies[1] = best_strategies[0];
                            best_strategies[0] = str;
                        
                        }

                        else if(str->score > best_strategies[1]->score){ 

                            best_strategies[2] = best_strategies[1];
                            best_strategies[1] = str; 
                        
                        }
                        
                        else{ best_strategies[2] = str; }                

                    }else{

                        if(str->score > best_strategies[2]->score){
                            
                            strategy_destroy(best_strategies[2]);
                            
                            if(str->score > best_strategies[1]->score){
                                
                                best_strategies[2] = best_strategies[1];
                                
                                if(str->score > best_strategies[0]->score){
                                    
                                    best_strategies[1] = best_strategies[0];
                                    
                                    best_strategies[0] = str;
                                }
                                else{ best_strategies[1] = str; }
                            }
                            
                            else{ best_strategies[2] = str; }
                        
                        }else{ strategy_destroy(str); }

                    }


                }

            }
            free_tetrimino(t);
        }
    }

    //ho le migliori tre strategie, ne ritorno una random
    for(i=0;i<3;i++){
        if(i != choosen){
            strategy_destroy(best_strategies[i]);
        }
    }
    set_field(g,best_strategies[choosen]->field);
    last_used_tet = best_strategies[choosen]->tet;
}

int calculate_score(int *field,int tet_type,int last_used_tet)
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
    
    if(last_used_tet == tet_type){
        score-= 30;
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
