#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <../include/player.h>
#include <../include/gamefield.h>
#include <../include/tetrimino.h>
#include <../include/functions.h>
#include <../include/constants.h>


void single_continueGame(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points);
void endGame(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves);
void newGameSingle();

void instructions(char* nickname){
    WINDOW *instructions_win;
    char ch;
    int i;
    char* welcome_TXT =     "Ciao, ";
    char* welcome2_TXT =    "Benvenuto su Tetrix, di seguito troverai le istruzioni per il gioco\n\n"
                            "Parte 1: \n"
                            "Usa le freccie UP e DOWN per muoverti nella pool di tetramini, premi ENTER per selezionarne uno\n\n"
                            "Parte 2:\nUsa le freccie LEFT e RIGHT per muovere il tetramino sopra la pool,\n"
                            "Premi freccia UP per ruotare il tetramino di 90 gradi, premi BACKSPACE per tornare alla selezione dei tetramini,\n"
                            "premi freccia DOWN per piazzarlo.\n\n"
                            "Punteggio e Game Over:\n"
                            "La rimozione di una riga vale 1 punto, la rimozione di due righe con un solo pezzo vale 3 punti, tre righe 6 punti,\n"
                            "quattro righe 12 punti.\n"
                            "Il gioco termina quando finiscono i pezzi o il giocatore non riesce a posizionare un tetramino nel campo di gioco\n"
                            "rispettando il limite di altezza.";
    int art_cols = getASCIIArtRows(ART_LOGO);
    /* 3 è l'altezza della finestra dei credits */
    instructions_win = newwin( LINES - art_cols - 3, COLS, 6 , 0 );
    box(instructions_win, 0, 0 );
    mvwprintw(instructions_win,0,1," BENVENUTO ");
    
    wmove(instructions_win,2,2);
    wprintWithDelay(instructions_win,20,welcome_TXT);
    
    while(nickname[i] != '\0'){
        
        wprintw(instructions_win,"%c",nickname[i]);
        i++;
        
        wrefresh(instructions_win);
        delay(20);
    }

    wmove(instructions_win,3,2);
    wprintWithDelay(instructions_win,20,welcome2_TXT);
    
    delay(1000);

    wmove(instructions_win,18,(COLS/2)-4);
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
 * @brief inizia una partita in single player
 */
void newGameSingle(){
    player_t *player;
    gamefield_t *gameField;
    tetrimini_pool_t *pool;
    pointboard_t *points;
    
    char *nickname = (char*) malloc(sizeof(char) * 16);
    form(nickname, 16, " Nome: ");
    refresh();

    instructions(nickname);

    player = initializePlayer(nickname);
    /* FIXME STO ANCORA SISTEMANDO, MI È SALITO SONNO */
    gameField = initializeGameField(7, (COLS - POOL_COLS - POINTBOARD_COLS - FIELD_W_COLS) / 2);
    pool = initializePool(6, 0);
    points = initializePointBoard(10, COLS - 28, player, NULL);
    single_continueGame(player, gameField, pool, points);
    return;
}

/**
 * @brief corpo principale del gioco che si alterna fra scelta blocco e drop dello stesso
 * 
 * @param[in] player Il giocatore.
 * @param[in] gameField Il campo da gioco del giocatore.
 * @param[in] pool I tetramini rimanenti.
 * @param[in] points Il punteggio del giocatore.
 */
void single_continueGame(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points){
    int selected_i, can_play = 1;
    tetrimino_t *selected_t;
    unsigned int start_time = time(NULL);
    int moves=0;

    while (can_play)
    {   
        int dropping = 1, cursor,i,j,deletedRows=0, skip = 1;
        int* field = getGamefield(gameField);
        selected_i = selectTetrimino(pool);
        selected_t = getTetrimino(selected_i);
        cursor = (FIELD_COLS - getTetCols(selected_t)) / 2;
        refreshSelector(gameField, selected_t, cursor);
        while (dropping)
        {
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    /*Muove il tetramino a destra*/
                    if (getTetCols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case KEY_LEFT:
                    /*Muove il tetramino a sinistra*/
                    if (cursor > 0)
                        --cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case KEY_UP:
                    /*ruota matrice di 90 gradi*/
                    safeRotateTetrimino(selected_t, cursor);
                    refreshSelector(gameField, selected_t, cursor);
                break;
                case KEY_DOWN:
                    /*Droppa il tetramino*/
                    dropping = 0;
                    clearTop(gameField);
                    addTetriminoToGameField(gameField,selected_t,cursor);
                    refreshGamefield(gameField);
                    break;
                case KEY_BACKSPACE:
                    /*Annulla la selezione*/
                    clearTop(gameField);
                    refreshGamefield(gameField);
                    
                    ch=-1;
                    dropping = 0;
                    continue;
            }
        }
        
        /*Aggiorna il counter delle mosse del giocatore*/
        moves++;
        /*Droppato un tetramino verifico se le righe sono state riempite*/
        for(i=0;i<FIELD_ROWS;++i){
            int empty=0;
            for(j=0;j<FIELD_COLS;++j){
                if(!field[i * FIELD_COLS + j]){
                    empty=1;
                    break;
                }
            }
            if(!empty){
                int k,l;
                mvwprintw(getGamefieldWin(gameField), i + 4, 1, "====================");
                wrefresh(getGamefieldWin(gameField));
                delay(100);
                deletedRows++;
                for(k=i;k>0;--k){
                    for(l=0;l<FIELD_COLS;++l){
                        field[k*FIELD_COLS+l] = field[(k-1)*FIELD_COLS+l];
                    }
                    refreshGamefield(gameField);
                    delay(50); /*la funzione in realtá blocca di fatti tutto il programma per 50 millisecondi*/
                }
            }
        }
        
        /*aggiungo i punti*/
        switch (deletedRows)
        {
        case 1:
            playerAddPoints(player,points,POINTS_ONE_ROW_DELETED);
            break;
        case 2:
            playerAddPoints(player,points,POINTS_TWO_ROW_DELETED);
            break;
        case 3:
            playerAddPoints(player,points,POINTS_THREE_ROW_DELETED);
            break;
        case 4:
            playerAddPoints(player,points,POINTS_FOUR_ROW_DELETED);
            break;
        default:
            break;
        }
        
        /*resetto le righe eliminate nel turno*/
        deletedRows = 0;
        dropping = 0;
        freeTetrimino(selected_t);
        /*verifico che ci siano ancora le condizioni per giocare*/
        if(noTetriminosLeft(pool) || gameFieldTopIsOccupied(gameField)){
            can_play = 0;
        }
    }
    endGame(gameField,pool,points, player, start_time,moves);
}

void endGame(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves)/*thanos++*/{
    WINDOW* fieldWin = getGamefieldWin(gameField);
    WINDOW* poolWin = getPoolWin(pool);
    WINDOW* pointWin = getPointBoardWin(points);
    WINDOW *summary = newwin( 14, COLS-2, (LINES/2)-5 , 1 );

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    char* thanks_TXT = "GRAZIE PER AVER GIOCATO A TETRIX, ";
    char* nickname = getPlayerNick(player);
    char* stats_TXT =   "ECCO LE TUE STATISTICHE: \n\n"
                        "Punteggio totale:    ";
    unsigned int playerPoints = getPlayerPoints(player);
    char* matchTime_TXT = "Durata del match:    ";
    char* moves_TXT = "Turni di gioco:      ";

    int ypos = (getmaxy(summary) - getASCIIArtRows(ART_GAME_OVER)) / 2;
    int xpos = (getmaxx(summary) - getASCIIArtCols(ART_GAME_OVER)) / 2;

    killWin(fieldWin);
    killWin(poolWin);
    killWin(pointWin);

    box(summary, 0, 0 );
    mvwprintwrows(summary, ypos, xpos, ART_GAME_OVER);
    wrefresh(summary);
    delay(1000);
    wclear(summary);
    box(summary, 0, 0 );
    wrefresh(summary);
    
    wmove(summary,2,2);
    wprintWithDelay(summary,20,thanks_TXT);
    
    i=0;
    while(nickname[++i] != '\0'){
        if(nickname[i] !='\0'){
            wprintw(summary,"%c",nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(20);
    }
    
    wmove(summary,3,2);
    wprintWithDelay(summary,20,stats_TXT);
    
    wprintw(summary,"%05u",playerPoints);
    wrefresh(summary);
    
    delay(500);
    
    wmove(summary,7,2);
    wprintWithDelay(summary,20,matchTime_TXT);
    
    wprintw(summary,"%05u",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,9,2);
    wprintWithDelay(summary,20,moves_TXT);
    
    wprintw(summary,"%05d",moves);
    wrefresh(summary);
    
    delay(500);

    wmove(summary,12,(COLS/2)-9);
    wattron(summary, A_STANDOUT );
    wprintw(summary,"> Torna al menu! <");
    wattroff(summary, A_STANDOUT );
    wrefresh(summary);

    freeGamefield(gameField);
    freePool(pool);
    freePointBoard(points);

    ch = -1;

    do{
        ch = wgetch(summary);
    }while(ch != 10);

    killWin(summary);
}
