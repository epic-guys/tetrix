#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <../include/constants.h>

void pvp_continueGame();
void pvp_endGame();
void newGameMulti();

void pvp_instructions(char* nickname1, char* nickname2){
    WINDOW *instructions_win;
    char ch;
    int art_cols = getASCIIArtRows(ART_LOGO);
    instructions_win = newwin( LINES - art_cols - 3, COLS, 6 , 0 );
    box(instructions_win, 0, 0 );
    mvwprintw(instructions_win,0,1," BENVENUTI ");

    wmove(instructions_win,2,2);
    wprintWithDelay(instructions_win,20,PVP_WELCOME_TXT[0]);
    wprintWithDelay(instructions_win,20,nickname1);
    wprintWithDelay(instructions_win,20," e ");
    wprintWithDelay(instructions_win,20,nickname2);
    wmove(instructions_win,3,2);
    wprintWithDelay(instructions_win,20,PVP_WELCOME_TXT[1]);

    delay(1000);

    wmove(instructions_win,19,(COLS/2)-4);
    wattron(instructions_win, A_STANDOUT );
    wprintw(instructions_win,"> Gioca! <");
    wattroff(instructions_win, A_STANDOUT );
    wrefresh(instructions_win);

    ch = -1;
    do{
        ch = wgetch(instructions_win);
    }while(ch != 10);

    killWin(instructions_win);
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

    nickname1 = form(16, " Nome 1: ");
    refresh();
    nickname2 = form(16, " Nome 2: ");
    refresh();

    pvp_instructions(nickname1, nickname2);

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
    srand(time(NULL));
    turn = rand()%2;
    
    while (can_play[0] && can_play[1])
    {
        int dropping = 1, cursor,i,j,deletedRows=0, skip = 1,backspace_pressed=0;
        int *currentField;
        mvprintw(11, (COLS/2)-(POOL_COLS/2)-19, "                                ");
        mvprintw(11, (COLS/2)-(POOL_COLS/2)-19, "Turno di: %s", getPlayerNick(players[turn]));
        refresh();

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
                    moves[turn]++;
                    break;
                case 127:
                    /*Annulla la selezione*/
                    clearTop(gameFields[turn]);
                    refreshGamefield(gameFields[turn]);
                    backspace_pressed=1;
                    ch=-1;
                    dropping = 0;
                    //continue;
                    break;
            }

            /*Aggiorna il counter delle mosse del giocatore*/
            if(!backspace_pressed){
                moves[turn] = moves[turn] + 1;
            }

            /*Droppato un tetramino verifico se le righe sono state riempite*/
            for(i = 0; i < FIELD_ROWS; ++i)
            {
                if(isRowFull(gameFields[turn], i)){
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
        case 3:
            for(i=0;i<3;++i){
                flipValuesInRow(gameFields[1 - turn], i);
            }
            refreshGamefield(gameFields[1 - turn]);
            break;
        case 4:
            for(i=0;i<3;++i){
                flipValuesInRow(gameFields[1 - turn], i);
            }
            refreshGamefield(gameFields[1 - turn]);
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

    /*Dio dell'informatica perdonami per questa patch indegna,
      credo sia colpa delle FS se sto veramente per pushare*/
    moves[0] = moves[0]/6 + 1;
    moves[1] = moves[1]/6 + 1;
    char* p1_nickname = getPlayerNick(players[0]);
    char* p2_nickname = getPlayerNick(players[1]);
    unsigned int playersPoints[2] = { getPlayerPoints(players[0]), getPlayerPoints(players[1])};
    killWin(field1Win);
    killWin(field2Win);
    killWin(poolWin);
    killWin(pointWin);
    mvprintw(11, (COLS/2)-(POOL_COLS/2)-19, "                                ");
    refresh();

    summary = newwin( 18, COLS-2, (LINES/2) -5, 1 );
    box(summary, 0, 0 );
    mvwprintw(summary,0,1," GAME OVER ");
    
    wmove(summary,2,2);
    wprintWithDelay(summary,20,PVP_END_TXT[0]);
    wprintWithDelay(summary,20,PVP_END_TXT[1]);
    wprintWithDelay(summary,20,p1_nickname);
    wprintWithDelay(summary,20," : ");
    wprintw(summary,"%05u",playersPoints[0]);
    wmove(summary,6,2);
    wprintWithDelay(summary,20,PVP_END_TXT[1]);
    wprintWithDelay(summary,20,p2_nickname);
    wprintWithDelay(summary,20," : ");
    wprintw(summary,"%05u",playersPoints[1]);
    wrefresh(summary);
    
    delay(500);
    
    wmove(summary,8,2);
    wprintWithDelay(summary,20,PVP_END_TXT[2]);
    
    wprintw(summary,"%05u s",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,10,2);
    wprintWithDelay(summary,20,PVP_END_TXT[3]);
    wprintWithDelay(summary,20,p1_nickname);
    wprintWithDelay(summary,20," :      ");
    
    wprintw(summary,"%05d",moves[0]);
    wrefresh(summary);
    
    wmove(summary,11,2);
    wprintWithDelay(summary,20,PVP_END_TXT[3]);
    wprintWithDelay(summary,20,p2_nickname);
    wprintWithDelay(summary,20," :      ");
    
    wprintw(summary,"%05d",moves[1]);
    wrefresh(summary);

    wmove(summary,13,2);

    wprintWithDelay(summary,20,PVP_END_TXT[4]);

    i=0;

    if(!win_flag){
        wprintWithDelay(summary,20,p1_nickname);
    }
    else if(win_flag){
        wprintWithDelay(summary,20,p2_nickname);
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