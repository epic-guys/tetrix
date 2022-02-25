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
    srand(time(0));
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
            /*da aggiustare secondo specifiche del readme*/
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
        
        if(noTetriminosLeft(pool)){
            pvp_endGame(!turn,gameField1,gameField2,pool,points,player1,player2,start_time,p1_moves,p2_moves);
            return;
        }
        else if(gameFieldTopIsOccupied(turn==0 ? gameField1 : gameField2)){
            if(getPlayerPoints(player1)>getPlayerPoints(player2)){
                pvp_endGame(0,gameField1,gameField2,pool,points,player1,player2,start_time,p1_moves,p2_moves);
            }
            else if(getPlayerPoints(player1)>getPlayerPoints(player2)){
                pvp_endGame(1,gameField1,gameField2,pool,points,player1,player2,start_time,p1_moves,p2_moves);
            }
            else{
                pvp_endGame(!turn,gameField1,gameField2,pool,points,player1,player2,start_time,p1_moves,p2_moves);
            }
            return;
        }
        
        if(turn==0 && backspace_pressed==0){
            turn=1;
            p1_moves++;
        }
        else if(turn==1 && backspace_pressed==0){
            turn=0;
            p2_moves++;
        }

        backspace_pressed=0;
    }
    
}

/*da finire di aggiustare*/

void pvp_endGame(int win_flag,gamefield_t *gameField1, gamefield_t *gameField2, tetrimini_pool_t *pool, pointboard_t *points, player_t *player1, player_t *player2,unsigned int start_time,int p1_moves, int p2_moves)/*thanos++*/{
    WINDOW* field1Win = getGamefieldWin(gameField1);
    WINDOW* field2Win = getGamefieldWin(gameField2);
    WINDOW* poolWin = getPoolWin(pool);
    WINDOW* pointWin = getPointBoardWin(points);
    WINDOW *summary;

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    char* thanks_TXT = "GRAZIE PER AVER GIOCATO A TETRIX!";
    char* p1_nickname = getPlayerNick(player1);
    char* p2_nickname = getPlayerNick(player2);
    char* stats_TXT =   "ECCO LE VOSTRE STATISTICHE:";
    unsigned int player1Points = getPlayerPoints(player1);
    unsigned int player2Points = getPlayerPoints(player2);
    char* matchTime_TXT = "Durata del match:    ";

    killWin(field1Win);
    killWin(field2Win);
    killWin(poolWin);
    killWin(pointWin);

    summary = newwin( 18, COLS-2, (LINES/2) -5, 1 );
    box(summary, 0, 0 );
    mvwprintw(summary,0,1," GAME OVER ");
    
    wmove(summary,2,2);
    wprintWithDelay(summary,20,thanks_TXT);
    
    wmove(summary,4,2);
    wprintWithDelay(summary,20,stats_TXT);
    wmove(summary,5,2);
    wprintWithDelay(summary,20,"Punteggio ");
    i=0;
    while(p1_nickname[++i] != '\0'){
        if(p1_nickname[i] !='\0'){
            wprintw(summary,"%c",p1_nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(20);
    }
    wprintWithDelay(summary,20," : ");
    wprintw(summary,"%05u",player1Points);
    wmove(summary,6,2);
    wprintWithDelay(summary,20,"Punteggio ");
    i=0;
    while(p2_nickname[++i] != '\0'){
        if(p2_nickname[i] !='\0'){
            wprintw(summary,"%c",p2_nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(20);
    }
    wprintWithDelay(summary,20," : ");
    wprintw(summary,"%05u",player2Points);
    wrefresh(summary);
    
    delay(500);
    
    wmove(summary,8,2);
    wprintWithDelay(summary,20,matchTime_TXT);
    
    wprintw(summary,"%05u s",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,10,2);
    wprintWithDelay(summary,20,"Turni di gioco")
    i=0;
    while(p1_nickname[++i] != '\0'){
        if(p1_nickname[i] !='\0'){
            wprintw(summary,"%c",p1_nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(20);
    }
    wprintWithDelay(summary,20," :      ");
    
    wprintw(summary,"%05d",p1_moves);
    wrefresh(summary);
    
    wmove(summary,11,2);
    wprintWithDelay(summary,20,"Turni di gioco")
    i=0;
    while(p1_nickname[++i] != '\0'){
        if(p1_nickname[i] !='\0'){
            wprintw(summary,"%c",p1_nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(20);
    }
    wprintWithDelay(summary,20," :      ");
    
    wprintw(summary,"%05d",p2_moves);
    wrefresh(summary);

    wmove(summary,13,2);

    wprintw(summary,"VINCITORE: ");

    i=0;

    if(!win_flag){
        while(p1_nickname[++i] != '\0'){
            if(p1_nickname[i] !='\0'){
                wprintw(summary,"%c",p1_nickname[--i]);
                i++;
            }
        wrefresh(summary);
        }
    }
    else if(win_flag){
        while(p2_nickname[++i] != '\0'){
            if(p2_nickname[i] !='\0'){
                wprintw(summary,"%c",p2_nickname[--i]);
                i++;
            }
        wrefresh(summary);
        }
    }
    else{ wprintw(summary,"PAREGGIO"); wrefresh(summary);} /*In realtá non accade mai, almeno, dipende dalle disposizioni*/
    
    delay(500);

    wmove(summary,16,(COLS/2)-9);
    wattron(summary, A_STANDOUT );
    wprintw(summary,"> Torna al menu! <");
    wattroff(summary, A_STANDOUT );
    wrefresh(summary);

    freeGamefield(gameField1);
    freeGamefield(gameField2);
    freePool(pool);
    freePointBoard(points);

    ch = -1;

    do{
        ch = wgetch(summary);
    }while(ch != 10);

    killWin(summary);
    return;
}