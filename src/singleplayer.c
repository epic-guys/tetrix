#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <../include/player.h>
#include <../include/gamefield.h>
#include <../include/tetrimino.h>
#include <../include/functions.h>
#include <../include/constants.h>

void single_continue_game(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points);
void end_game(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves);
void new_game_single();

void instructions(char* nickname){
    WINDOW *instructions_win;
    char ch;
    int art_cols = get_ASCII_art_rows(ART_LOGO);
    /* 3 è l'altezza della finestra dei credits */
    instructions_win = newwin( LINES - art_cols - 3, COLS, 6 , 0 );
    box(instructions_win, 0, 0 );
    mvwprintw(instructions_win,0,1," BENVENUTO ");
    
    wmove(instructions_win,2,2);
    wprint_with_delay(instructions_win,20,SINGLE_WELCOME_TXT[0]);
    wprint_with_delay(instructions_win,20,nickname);
    wmove(instructions_win,3,2);
    wprint_with_delay(instructions_win,20,SINGLE_WELCOME_TXT[1]);
    
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

    kill_win(instructions_win);
    return;
}

/**
 * @brief inizia una partita in single player
 */
void new_game_single(){
    player_t *player;
    gamefield_t *gameField;
    tetrimini_pool_t *pool;
    pointboard_t *points;
    
    char *nickname;
    int logo_rows = get_ASCII_art_rows(ART_LOGO);
    nickname = form(16, " Nome: ");
    refresh();

    instructions(nickname);

    player = initialize_player(nickname);
    /*
    FIXME per qualche ragione il campo e la pool continuano
    a spostarsi troppo sotto quando il terminale è troppo piccolo
    */
    gameField = initialize_gamefield(
        (LINES + logo_rows - FIELD_W_ROWS - 3) / 2,
        (COLS + POOL_COLS - POINTBOARD_COLS - FIELD_W_COLS) / 2
        );
    pool = initialize_pool((LINES + logo_rows - POOL_ROWS) / 2, 0);
    points = initialize_pointboard(10, COLS - POINTBOARD_COLS, player, NULL);
    single_continue_game(player, gameField, pool, points);
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
void single_continue_game(player_t *player, gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points){
    int selected_i, can_play = 1;
    tetrimino_t *selected_t;
    unsigned int start_time = time(NULL);
    int moves=0;

    while (can_play)
    {   
        int dropping = 1, cursor,i,j,deletedRows=0, skip = 1;
        int* field = get_gamefield(gameField);
        selected_i = select_tetrimino(pool);
        selected_t = get_tetrimino(selected_i);
        cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;
        refresh_selector(gameField, selected_t, cursor);
        while (dropping)
        {
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    /*Muove il tetramino a destra*/
                    if (get_tet_cols(selected_t) + cursor < FIELD_COLS)
                        ++cursor;
                    refresh_selector(gameField, selected_t, cursor);
                    break;
                case KEY_LEFT:
                    /*Muove il tetramino a sinistra*/
                    if (cursor > 0)
                        --cursor;
                    refresh_selector(gameField, selected_t, cursor);
                    break;
                case KEY_UP:
                    /*ruota matrice di 90 gradi*/
                    safe_rotate_tetrimino(selected_t, cursor);
                    refresh_selector(gameField, selected_t, cursor);
                break;
                case KEY_DOWN:
                    /*Droppa il tetramino*/
                    dropping = 0;
                    clear_top(gameField);
                    add_tetrimino_to_gamefield(gameField,selected_t,cursor);
                    refresh_gamefield(gameField);
                    break;
                case KEY_BACKSPACE:
                    /*Annulla la selezione*/
                    clear_top(gameField);
                    refresh_gamefield(gameField);
                    add_tetrimino_from_pool(selected_i, pool);
                    
                    ch=-1;
                    dropping = 0;
                    moves--;
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
                mvwprintw(get_gamefield_win(gameField), i + 4, 1, "====================");
                wrefresh(get_gamefield_win(gameField));
                delay(100);
                deletedRows++;
                for(k=i;k>0;--k){
                    for(l=0;l<FIELD_COLS;++l){
                        field[k*FIELD_COLS+l] = field[(k-1)*FIELD_COLS+l];
                    }
                    refresh_gamefield(gameField);
                    delay(50); /*la funzione in realtá blocca di fatti tutto il programma per 50 millisecondi*/
                }
            }
        }
        
        /*aggiungo i punti*/
        switch (deletedRows)
        {
        case 1:
            player_add_points(player,points,POINTS_ONE_ROW_DELETED);
            break;
        case 2:
            player_add_points(player,points,POINTS_TWO_ROW_DELETED);
            break;
        case 3:
            player_add_points(player,points,POINTS_THREE_ROW_DELETED);
            break;
        case 4:
            player_add_points(player,points,POINTS_FOUR_ROW_DELETED);
            break;
        default:
            break;
        }
        
        /*resetto le righe eliminate nel turno*/
        deletedRows = 0;
        dropping = 0;
        free_tetrimino(selected_t);
        /*verifico che ci siano ancora le condizioni per giocare*/
        if(no_tetriminos_left(pool) || is_gamefield_top_occupied(gameField)){
            can_play = 0;
        }
    }
    end_game(gameField,pool,points, player, start_time,moves);
}

void end_game(gamefield_t *gameField, tetrimini_pool_t *pool, pointboard_t *points, player_t *player,unsigned int start_time,int moves)/*thanos++*/{
    WINDOW* fieldWin = get_gamefield_win(gameField);
    WINDOW* poolWin = get_pool_win(pool);
    WINDOW* pointWin = get_pointboard_win(points);
    WINDOW *summary = newwin( 14, COLS-2, (LINES/2)-5 , 1 );

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    char* nickname = get_player_nick(player);
    unsigned int playerPoints = get_player_points(player);

    int ypos = (getmaxy(summary) - get_ASCII_art_rows(ART_GAME_OVER)) / 2;
    int xpos = (getmaxx(summary) - get_ASCII_art_cols(ART_GAME_OVER)) / 2;

    kill_win(fieldWin);
    kill_win(poolWin);
    kill_win(pointWin);

    box(summary, 0, 0 );
    mvwprintwrows(summary, ypos, xpos, ART_GAME_OVER);
    wrefresh(summary);
    delay(1000);
    wclear(summary);
    box(summary, 0, 0 );
    wrefresh(summary);
    
    wmove(summary,2,2);
    wprint_with_delay(summary,20,SINGLE_END_TXT[0]);
    wprint_with_delay(summary,20,nickname);
    wmove(summary,3,2);
    wprint_with_delay(summary,20,SINGLE_END_TXT[1]);
    wprintw(summary,"%05u",playerPoints);
    wrefresh(summary);
    delay(500);
    wmove(summary,7,2);
    wprint_with_delay(summary,20,SINGLE_END_TXT[2]);
    
    wprintw(summary,"%05u",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,9,2);
    wprint_with_delay(summary,20,SINGLE_END_TXT[3]);
    
    wprintw(summary,"%05d",moves);
    wrefresh(summary);
    
    delay(500);

    wmove(summary,12,(COLS/2)-9);
    wattron(summary, A_STANDOUT );
    wprintw(summary,"> Torna al menu! <");
    wattroff(summary, A_STANDOUT );
    wrefresh(summary);

    free_gamefield(gameField);
    free_pool(pool);
    free_pointboard(points);

    ch = -1;

    do{
        ch = wgetch(summary);
    }while(ch != 10);

    kill_win(summary);
}
