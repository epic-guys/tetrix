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
    player_t **players = (player_t **) malloc(sizeof(player_t *) * 2);
    gamefield_t **gameFields = (gamefield_t **) malloc(sizeof(gamefield_t *) * 2);

    tetrimini_pool_t *pool;
    pointboard_t *points;

    char *nickname1 = (char*) malloc(sizeof(char) * 16);
    char *nickname2 = (char*) malloc(sizeof(char) * 16);

    form(nickname1, 16, " Nome 1: ");
    refresh();
    form(nickname2, 16, " Nome 2: ");
    refresh();

    players[0] = initializePlayer(nickname1);
    players[1] = initializePlayer(nickname2);    
    gameFields[0] = initializeGameField(12, 5);
    gameFields[1] = initializeGameField(12, (COLS/2)+(POOL_COLS/2));
    pool = initializePool(12, (COLS/2)-(POOL_COLS/2)-20);
    points = initializePointBoard(10, COLS - 30, players[0], players[1]);

    pvp_continueGame(players, gameFields, pool, points);
}

void pvp_continueGame(player_t **players, gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points){
    int can_play[2] = {1, 1};
    int selected_i;
    tetrimino_t *selected_t;
    unsigned int start_time = time(NULL),seed=time(0);
    int *moves = (int*)malloc(sizeof(int) * 2);
    moves[0] = 0;
    moves[1] = 0;
    int turn;

    /*Il giocatore che inizia é deciso random*/
    srand(time(0));
    turn = rand()%2;
    
    while (can_play[0] && can_play[1])
    {
        int dropping = 1, cursor,i,j,deletedRows=0, skip = 1,backspace_pressed=0;
        int *currentField;
        
        selected_i = selectTetrimino(pool);
        selected_t = getTetrimino(selected_i);
        cursor = (FIELD_COLS - getTetCols(selected_t)) / 2;

        currentField = getGamefield(gameFields[turn]);
        refreshSelector(gameFields[turn], selected_t, cursor);

        while(dropping){
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    /*Muove il tetramino a destra*/
                    if (getTetCols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                        refreshSelector(gameFields[turn], selected_t, cursor);

                    break;
                case KEY_LEFT:
                    /*Muove il tetramino a sinistra*/
                    if (cursor > 0)
                        --cursor;
                    refreshSelector(gameFields[turn], selected_t, cursor);
                    break;
                case KEY_UP:
                    /*ruota matrice di 90 gradi*/
                    safeRotateTetrimino(selected_t, cursor);
                    refreshSelector(gameFields[turn], selected_t, cursor);
                    break;
                case KEY_DOWN:
                    /*Droppa il tetramino*/
                    dropping = 0;
                    clearTop(gameFields[turn]);
                    addTetriminoToGameField(gameFields[turn],selected_t,cursor);
                    refreshGamefield(gameFields[turn]);
                    break;
                case KEY_BACKSPACE:
                    /*Annulla la selezione*/
                    clearTop(gameFields[turn]);
                    refreshGamefield(gameFields[turn]);
                    backspace_pressed=1;
                    ch=-1;
                    dropping = 0;
                    continue;
            }

            /*Aggiorna il counter delle mosse del giocatore*/
            moves[turn]++;

            /*Droppato un tetramino verifico se le righe sono state riempite*/
            for(i = 0; i < FIELD_ROWS; ++i)
            {
                if(is_row_full(gameFields[turn], i)){
                    int k,l;
                    mvwprintw(getGamefieldWin(gameFields[turn]), i + 4, 1, "====================");
                    wrefresh(getGamefieldWin(gameFields[turn]));
                    delay(100);
                    deletedRows++;
                    for(k=i;k>0;--k){
                        for(l=0;l<FIELD_COLS;++l){
                            currentField[k * FIELD_COLS + l] = currentField[(k-1)*FIELD_COLS+l];
                        }
                        refreshGamefield(gameFields[turn]);
                        delay(50); /*la funzione in realtá blocca di fatti tutto il programma per 50 millisecondi*/
                    }
                }
            }

        }

        /*aggiungo i punti*/
        switch (deletedRows)
        {
        case 1:
            playerAddPoints(players[turn], points, POINTS_ONE_ROW_DELETED);
            break;
        case 2:
            playerAddPoints(players[turn], points, POINTS_TWO_ROW_DELETED);
            break;
            /*da aggiustare secondo specifiche del readme*/
        case 3:
            playerAddPoints(players[turn], points, POINTS_THREE_ROW_DELETED);
            break;
        case 4:
            playerAddPoints(players[turn], points, POINTS_FOUR_ROW_DELETED);
            break;
        default:
            break;
        }

        /*resetto le righe eliminate nel turno*/
        deletedRows = 0;
        dropping = 0;

        /*verifico che ci siano ancora le condizioni per giocare*/
        if(noTetriminosLeft(pool) || gameFieldTopIsOccupied(gameFields[turn])){
            can_play[turn] = 0;
            
            freeTetrimino(selected_t);
        }

        if (noTetriminosLeft(pool))
        {
            int winner = getPlayerPoints(players[0]) < getPlayerPoints(players[1]);
            pvp_endGame(winner, gameFields, pool, points, players, start_time, moves);
            return;
        }
        if (gameFieldTopIsOccupied(gameFields[turn]))
        {
            pvp_endGame(1 - turn, gameFields, pool, points, players, start_time, moves);
            return;
        }

        if (!backspace_pressed)
        {
            moves[turn]++;
            turn = 1 - turn;
        }

        backspace_pressed=0;
    }
    
}

/*da finire di aggiustare*/

void pvp_endGame(int win_flag,gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points, player_t **players,unsigned int start_time,int *moves)/*thanos++*/{
    WINDOW* field1Win = getGamefieldWin(gameFields[0]);
    WINDOW* field2Win = getGamefieldWin(gameFields[1]);
    WINDOW* poolWin = getPoolWin(pool);
    WINDOW* pointWin = getPointBoardWin(points);
    WINDOW *summary;

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    char* thanks_TXT = "GRAZIE PER AVER GIOCATO A TETRIX!";
    char* p1_nickname = getPlayerNick(players[0]);
    char* p2_nickname = getPlayerNick(players[1]);
    char* stats_TXT =   "ECCO LE VOSTRE STATISTICHE:";
    unsigned int playersPoints[2] = { getPlayerPoints(players[0]), getPlayerPoints(players[1])};
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
    wprintw(summary,"%05u",playersPoints[0]);
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
    wprintw(summary,"%05u",playersPoints[1]);
    wrefresh(summary);
    
    delay(500);
    
    wmove(summary,8,2);
    wprintWithDelay(summary,20,matchTime_TXT);
    
    wprintw(summary,"%05u s",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,10,2);
    wprintWithDelay(summary,20,"Turni di gioco");
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
    
    wprintw(summary,"%05d",moves[0]);
    wrefresh(summary);
    
    wmove(summary,11,2);
    wprintWithDelay(summary,20,"Turni di gioco");
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
    
    wprintw(summary,"%05d",moves[1]);
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

    freePlayer(players[0]);
    freePlayer(players[1]);
    free(players);
    freeGamefield(gameFields[0]);
    freeGamefield(gameFields[1]);
    free(gameFields);
    freePool(pool);
    freePointBoard(points);
    free(moves);

    ch = -1;

    do{
        ch = wgetch(summary);
    }while(ch != 10);

    killWin(summary);
    return;
}

//dopo lo cancello sto provando
