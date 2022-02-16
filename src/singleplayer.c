#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <constants.h>

void continue_game(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points);
void end_game(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves);
void newGameSingle();

void instructions(char* nickname){
    WINDOW *instructions_win;
    char ch;
    int i;
    char* welcome_TXT = "Ciao, ";
    char* welcome2_TXT = "Benvenuto su Tetrix, di seguito troverai le istruzioni per il gioco\n\nParte 1: \nUsa le freccie UP e DOWN per muoverti nella pool di tetramini, premi ENTER per selezionarne uno\n\nParte 2:\nUsa le freccie LEFT e RIGHT per muovere il tetramino sopra la pool,\nPremi freccia UP per ruotare il tetramino di 90 gradi, premi BACKSPACE per tornare alla selezione dei tetramini, premi freccia DOWN per piazzarlo.\n\nPunteggio e Game Over: \nLa rimozione di una riga vale 1 punto, la rimozione di due righe con un solo pezzo vale 3 punti, tre righe 6 punti, quattro righe 12 punti. \nIl gioco termina quando finiscono i pezzi o il giocatore non riesce a posizionare un tetramino nel campo di gioco rispettando il limite di altezza.";
    instructions_win = newwin( 20, COLS-2, (LINES/2)-5 , 1 );
    box(instructions_win, 0, 0 );
    mvwprintw(instructions_win,0,1," BENVENUTO ");
    
    wmove(instructions_win,2,2);
    wprintWithDelay(instructions_win,300,welcome_TXT);
    
    while(nickname[++i] != '\0'){
        if(nickname[i] !='\0'){
            wprintw(instructions_win,"%c",nickname[--i]);
            i++;
        }
        wrefresh(instructions_win);
        delay(300);
    }

    wmove(instructions_win,3,2);
    wprintWithDelay(instructions_win,300,welcome2_TXT);
    
    wmove(instructions_win,5,2);
    //wprintWithDelay(instructions_win,300,points_TXT);
    
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
    
    char *nickname = (char*) calloc(sizeof(char),16);
    form(nickname, 16, " Nome: ");
    refresh();

    instructions(nickname);

    player = initializePlayer(nickname);
    gameField = initializeGameField(12, (COLS/2)-(POOL_COLS/2)+(POOL_COLS/4));
    pool = initializePool(10, 4);
    points = initializePointBoard(10, COLS - 30, player, NULL);
    continue_game(player, gameField, pool, points);
}

/**
 * @brief corpo principale del gioco che si alterna fra scelta blocco e drop dello stesso
 * 
 * @param[in] player Il giocatore.
 * @param[in] gameField Il campo da gioco del giocatore.
 * @param[in] pool I tetramini rimanenti.
 * @param[in] points Il punteggio del giocatore.
 */
void continue_game(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points)
{
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
        cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;
        refreshSelector(gameField, selected_t, cursor);
        while (dropping)
        {
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    /*Muove il tetramino a destra*/
                    if (get_tet_cols(selected_t) + cursor < FIELD_COLS)
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
                    skip = 0;
                    ch=-1;
                    dropping = 0;
                    break;
            }
        }
        if(!skip){
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
                    deletedRows++;
                    for(k=i;k>0;--k){
                        for(l=0;l<FIELD_COLS;++l){
                            field[k*FIELD_COLS+l] = field[(k-1)*FIELD_COLS+l];
                        }
                        refreshGamefield(gameField);
                        delay(100); /*la funzione in realtá blocca di fatti tutto il programma per 100 millisecondi*/
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
            deletedRows = 0;

        }
        /*verifico che ci siano ancora le condizioni per giocare*/dropping = 0;
        if(noTetriminosLeft(pool) || gameFieldTopIsOccupied(gameField)){
            can_play = 0;
            freeTetrimino(selected_t);
        }
    }
    end_game(gameField,pool,points, player, start_time,moves);
}

void end_game(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves)/*thanos++*/
{
    WINDOW* fieldWin = getGamefieldWin(gameField);
    WINDOW* poolWin = getPoolWin(pool);
    WINDOW* pointWin = getPointBoardWin(points);
    WINDOW *summary;

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    char* thanks_TXT = "GRAZIE PER AVER GIOCATO A TETRIX, ";
    char* nickname = getPlayerNick(player);
    char* stats_TXT = "ECCO LE TUE STATISTICHE: ";
    char* points_TXT = "Punteggio totale:    ";
    unsigned int playerPoints = getPlayerPoints(player);
    char* matchTime_TXT = "Durata del match:    ";
    char* moves_TXT = "Turni di gioco:      ";


    killWin(fieldWin);
    killWin(poolWin);
    killWin(pointWin);

    summary = newwin( 14, COLS-2, (LINES/2)-5 , 1 );
    box(summary, 0, 0 );
    mvwprintw(summary,0,1," GAME OVER ");
    
    wmove(summary,2,2);
    wprintWithDelay(summary,300,thanks_TXT);
    
    i=0;
    while(nickname[++i] != '\0'){
        if(nickname[i] !='\0'){
            wprintw(summary,"%c",nickname[--i]);
            i++;
        }
        wrefresh(summary);
        delay(300);
    }
    
    wmove(summary,3,2);
    wprintWithDelay(summary,300,stats_TXT);
    
    wmove(summary,5,2);
    wprintWithDelay(summary,300,points_TXT);
    
    wprintw(summary,"%05u",playerPoints);
    wrefresh(summary);
    
    delay(1000);
    
    wmove(summary,7,2);
    wprintWithDelay(summary,300,matchTime_TXT);
    
    wprintw(summary,"%05u s",(end_time-start_time));
    wrefresh(summary);

    delay(1000);
    
    wmove(summary,9,2);
    wprintWithDelay(summary,300,moves_TXT);
    
    wprintw(summary,"%05d",moves);
    wrefresh(summary);
    
    delay(1000);

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