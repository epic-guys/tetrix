#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <constants.h>

/*
Potrebbero creare problemi in caso in un altro file si creano
variabili con lo stesso nome

player_t *player;
gamefield_t *gameField;
tetrimini_pool_t *pool;
pointboard_t *points;
*/

void continue_game();
void end_game();

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
    
    while (can_play)
    {   
        int* field = getGamefield(gameField);
        int dropping = 1, cursor,i,j,deletedRows=0;
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
                    if (get_tet_cols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case KEY_LEFT:
                    if (cursor > 0)
                        --cursor;
                    refreshSelector(gameField, selected_t, cursor);
                    break;
                case KEY_UP:
                    /*ruota matrice di 90 gradi*/
                    /*mvprintw(2,3,"%d",get_tet_type(selected_t)); LA LASCIO PER COMODITÁ*/
                    safeRotateTetrimino(selected_t, cursor);
                    /*mvprintw(6,3,"%d",get_tet_type(selected_t)); LA LASCIO PER COMODITÁ*/
                    refreshSelector(gameField, selected_t, cursor);
                break;
                case KEY_DOWN:
                    dropping = 0;
                    clearTop(gameField);
                    addTetriminoToGameField(gameField,selected_t,cursor);
                    refreshGamefield(gameField);
                    break;
            }
        }
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

        /*verifico che ci siano ancora le condizioni per giocare*/
        if(noTetriminosLeft(pool) || gameFieldTopIsOccupied(gameField)){
            can_play = 0;
            free(selected_t);
        }
    }
    end_game(gameField,pool,points, player);
}

void end_game(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player)/*thanos++*/
{
    WINDOW* fieldWin = getGamefieldWin(gameField);
    WINDOW* poolWin = getPoolWin(pool);
    WINDOW* pointWin = getPointBoardWin(points);
    WINDOW *summary;
    int i;
    char* thanks_TXT = "GRAZIE PER AVER GIOCATO A TETRIX, ";
    char* nickname = getPlayerNick(player);
    char* stats_TXT = "ECCO LE TUE STATISTICHE: ";
    char* points_TXT = "Punteggio totale:    ";
    int playerPoints = getPlayerPoints(player);
    char* matchTime_TXT = "Durata del match:    ";

    wclear(fieldWin);
    wrefresh(fieldWin);
    delwin(fieldWin);

    wclear(poolWin);
    wrefresh(poolWin);
    delwin(poolWin);

    wclear(pointWin);
    wrefresh(pointWin);
    delwin(pointWin);

    /*TODO: VA FATTA UNA FUNZIONE PER TUTTI QUESTI WHILE CHE FANNO SCHIFO... PERÓ É FIGHISSIMO IN STILE UNDERTALE, LOL*/
    summary = newwin( LINES/4 , COLS-2, (LINES/2)-5 , 1 );
    box(summary, 0, 0 );
    mvwprintw(summary,0,1," GAME OVER ");
    wmove(summary,2,2);
    //wprintw(summary,"GRAZIE PER AVER GIOCATO A TETRIX, %s !", getPlayerNick(player) );
    while(thanks_TXT[i] != '\0'){
        wprintw(summary,"%c",thanks_TXT[i++]);
        wrefresh(summary);
        delay(300);
    }

    i=0;
    /*while(nickname[i] != '\0'){
        if(nickname[i] !='\0')
            wprintw(summary,"%c",nickname[i++]);
        wrefresh(summary);
        delay(300);
    }*/
    
    i=0;
    wprintw(summary,"!");
    wrefresh(summary);
    delay(300);

    wmove(summary,3,2);
    while(stats_TXT[i] != '\0'){
        wprintw(summary,"%c",stats_TXT[i++]);
        wrefresh(summary);
        delay(300);
    }

    i=0;
    wmove(summary,5,2);
    while(points_TXT[i] != '\0'){
        wprintw(summary,"%c",points_TXT[i++]);
        wrefresh(summary);
        delay(300);
    }

    wprintw(summary,"%05d",playerPoints);
    wrefresh(summary);
    delay(1000);

    i=0;
    wmove(summary,7,2);
    while(matchTime_TXT[i] != '\0'){
        wprintw(summary,"%c",matchTime_TXT[i++]);
        wrefresh(summary);
        delay(300);
    }

    wprintw(summary,"NULL!");
    wrefresh(summary);
    delay(1000);

    while (true)
    {
        /* code */
    }
    

    /* TODO */
    /*free(nickname); PER RICORDARSI, ALTRIMENTI FACCIAMO UN MEMORY LEAK*/

    //poi va aggiornato per tornare al menu, ora non ho sbatti e mi serve per non buggare zsh, lol
    endwin();
}