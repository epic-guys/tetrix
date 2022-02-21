#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <constants.h>

void pvp_continueGame();
void pvp_endGame();
void newGameMulti();

void pvp_instructions(){
    return;
}

/**
 * @brief Inizia la partita in multiplayer
 * 
 */
void newGameMulti(){
    player_t *player1;
    player_t *player2;
    gamefield_t *gameField1;
    gamefield_t *gameField2;
    tetrimini_pool_t *pool;
    pointboard_t *points;

    char *nickname1 = (char*) calloc(sizeof(char),16);
    char *nickname2 = (char*) calloc(sizeof(char),16);

    form(nickname1, 16, " Nome 1: ");
    refresh();
    form(nickname2, 16, " Nome 2: ");
    refresh();

    player1 = initializePlayer(nickname1);
    player2 = initializePlayer(nickname2);
    gameField1 = initializeGameField(12, 5);
    gameField2 = initializeGameField(12, (COLS/2)+(POOL_COLS/2));
    pool = initializePool(12, (COLS/2)-(POOL_COLS/2)-20);
    points = initializePointBoard(10, COLS - 30, player1, player2);

    pvp_continueGame(player1, player2, gameField1, gameField2, pool, points);
}

void pvp_continueGame(player_t *player1, player_t *player2, gamefield_t *gameField1, gamefield_t *gameField2, tetrimini_pool_t *pool, pointboard_t *points){
    int p1_can_play =1, p2_can_play =1,selected_i;
    tetrimino_t *selected_t;
    unsigned int start_time = time(NULL),seed=time(0);
    int p1_moves=0;
    int p2_moves=0;

    /*Il giocatore che inizia é deciso random*/
    int turn = rand()%2;
    
    while (p1_can_play && p2_can_play)
    {
        int dropping = 1, cursor,i,j,deletedRows=0, skip = 1,backspace_pressed=0;
        int *field1, *field2;
        
        selected_i = selectTetrimino(pool);
        selected_t = getTetrimino(selected_i);
        cursor = (FIELD_COLS - getTetCols(selected_t)) / 2;
        
        if(turn==0){
            field1 = getGamefield(gameField1);
            refreshSelector(gameField1, selected_t, cursor);
        }
        else if(turn==1){
            field2 = getGamefield(gameField2);
            refreshSelector(gameField2, selected_t, cursor);
        }

        while(dropping){
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    /*Muove il tetramino a destra*/
                    if (getTetCols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                        refreshSelector(turn==0 ? gameField1 : gameField2, selected_t, cursor);

                    break;
                case KEY_LEFT:
                    /*Muove il tetramino a sinistra*/
                    if (cursor > 0)
                        --cursor;
                    refreshSelector(turn==0 ? gameField1 : gameField2, selected_t, cursor);
                    break;
                case KEY_UP:
                    /*ruota matrice di 90 gradi*/
                    safeRotateTetrimino(selected_t, cursor);
                    refreshSelector(turn==0 ? gameField1 : gameField2, selected_t, cursor);
                break;
                case KEY_DOWN:
                    /*Droppa il tetramino*/
                    dropping = 0;
                    clearTop(turn==0 ? gameField1 : gameField2);
                    addTetriminoToGameField(turn==0 ? gameField1 : gameField2,selected_t,cursor);
                    refreshGamefield(turn==0 ? gameField1 : gameField2);
                    break;
                case KEY_BACKSPACE:
                    /*Annulla la selezione*/
                    clearTop(turn==0 ? gameField1 : gameField2);
                    refreshGamefield(turn==0 ? gameField1 : gameField2);
                    backspace_pressed=1;
                    ch=-1;
                    dropping = 0;
                    continue;
            }

            /*Aggiorna il counter delle mosse del giocatore*/
            turn==0 ? p1_moves++ : p2_moves++;

            /*Droppato un tetramino verifico se le righe sono state riempite*/
            for(i=0;i<FIELD_ROWS;++i){
                int empty=0;
                for(j=0;j<FIELD_COLS;++j){
                    if(turn==0){
                        if(!field1[i * FIELD_COLS + j]){
                            empty=1;
                            break;
                        }
                    }
                    else{
                        if(!field2[i * FIELD_COLS + j]){
                            empty=1;
                            break;
                        }
                    }
                }
                if(!empty){
                    int k,l;
                    mvwprintw(getGamefieldWin(turn==0 ? gameField1 : gameField2), i + 4, 1, "====================");
                    wrefresh(getGamefieldWin(turn==0 ? gameField1 : gameField2));
                    delay(100);
                    deletedRows++;
                    for(k=i;k>0;--k){
                        for(l=0;l<FIELD_COLS;++l){
                            if(turn==0){
                                field1[k*FIELD_COLS+l] = field1[(k-1)*FIELD_COLS+l];
                            }
                            else{
                                field2[k*FIELD_COLS+l] = field2[(k-1)*FIELD_COLS+l];
                            }
                        }
                        refreshGamefield(turn==0 ? gameField1 : gameField2);
                        delay(50); /*la funzione in realtá blocca di fatti tutto il programma per 50 millisecondi*/
                    }
                }
            }

        }

        /*aggiungo i punti*/
        switch (deletedRows)
        {
        case 1:
            playerAddPoints(turn==0 ? player1 : player2,points,POINTS_ONE_ROW_DELETED);
            break;
        case 2:
            playerAddPoints(turn==0 ? player1 : player2,points,POINTS_TWO_ROW_DELETED);
            break;
        case 3:
            playerAddPoints(turn==0 ? player1 : player2,points,POINTS_THREE_ROW_DELETED);
            break;
        case 4:
            playerAddPoints(turn==0 ? player1 : player2,points,POINTS_FOUR_ROW_DELETED);
            break;
        default:
            break;
        }
        
        /*resetto le righe eliminate nel turno*/
        deletedRows = 0;
        dropping = 0;

        /*verifico che ci siano ancora le condizioni per giocare*/
        if(noTetriminosLeft(pool) || gameFieldTopIsOccupied(turn==0 ? gameField1 : gameField2)){
            if(turn==0){
                p1_can_play=0;
            }
            else{
                p2_can_play=0;
            }
            freeTetrimino(selected_t);
        }
        
        if(turn==0 && backspace_pressed==0){
            turn=1;
        }
        else if(turn==1 && backspace_pressed==0){
            turn=0;
        }

        backspace_pressed=0;
    }
    
}
