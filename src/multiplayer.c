#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <player.h>
#include <gamefield.h>
#include <tetrimino.h>
#include <functions.h>
#include <../include/constants.h>

void pvp_continue_game();
void pvp_end_game();
void new_game_multi();

void pvp_instructions(char* nickname1, char* nickname2){
    WINDOW *instructions_win;
    char ch;
    int art_cols = get_ASCII_art_rows(ART_LOGO);
    instructions_win = newwin( LINES - art_cols - 3, COLS, 6 , 0 );
    box(instructions_win, 0, 0 );
    mvwprintw(instructions_win,0,1," BENVENUTI ");

    wmove(instructions_win,2,2);
    wprint_with_delay(instructions_win,20,PVP_WELCOME_TXT[0]);
    wprint_with_delay(instructions_win,20,nickname1);
    wprint_with_delay(instructions_win,20," e ");
    wprint_with_delay(instructions_win,20,nickname2);
    wmove(instructions_win,3,2);
    wprint_with_delay(instructions_win,20,PVP_WELCOME_TXT[1]);

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

    kill_win(instructions_win);
    return;
}

/**
 * @brief Inizia la partita in multiplayer
 * 
 */
void new_game_multi(){
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

    players[0] = initialize_player(nickname1);
    players[1] = initialize_player(nickname2);

    gameFields[0] = initialize_gamefield(6, 5); /*inizialmente era così: 12, 5 */
    gameFields[1] = initialize_gamefield(6, (COLS/2)+(POOL_COLS/2)+5); /*inizialmente era così: 12, (COLS/2)+(POOL_COLS/2)*/
    pool = initialize_pool(6, (COLS/2)-(POOL_COLS/2)-3); /*inizialmente è così: 12, (COLS/2)-(POOL_COLS/2)-20*/
    points = initialize_pointboard(0, COLS - 30, players[0], players[1]); /*inizialmente era così: 10, COLS - 30, players[0], players[1]*/

    pvp_continue_game(players, gameFields, pool, points);
}

void pvp_continue_game(player_t **players, gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points){
    int selected_i;
    int winner = -1;
    tetrimino_t *selected_t;
    unsigned int start_time = time(NULL),seed=time(0);
    int *moves = (int*)malloc(sizeof(int) * 2);
    moves[0] = 0;
    moves[1] = 0;
    int turn;

    /*Il giocatore che inizia é deciso random*/
    srand(time(NULL));
    turn = rand()%2;
    
    while (winner < 0)
    {
        int dropping = 1, cursor,deletedRows=0, added = 1;
        int *currentField;
        mvprintw(5,0, "                                "); /*inizialmente era così: 11, (COLS/2)-(POOL_COLS/2)-19*/
        mvprintw(5,0, "Turno di: %s", get_player_nick(players[turn])); /*inizialmente era così: 11, (COLS/2)-(POOL_COLS/2)-19*/
        refresh();

        selected_i = select_tetrimino(pool);
        selected_t = get_tetrimino(selected_i);
        cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;

        currentField = get_gamefield(gameFields[turn]);
        refresh_selector(gameFields[turn], selected_t, cursor);
        cursor = manage_drop(gameFields[turn], selected_t);
        
        if (cursor >= 0)
        {
            added = add_tetrimino_to_gamefield(gameFields[turn], selected_t, cursor);
            remove_tetrimino_from_pool(selected_i, pool);
            moves[turn]++;

            if (added)
            {
                deletedRows = check_field(gameFields[turn]);
                if (deletedRows > 0)
                {
                    player_add_points(players[turn], points, get_points(deletedRows));
                    if (deletedRows >= 3)
                    {
                        flip_values(gameFields[1 - turn], deletedRows);
                    }
                }
                if (no_tetriminos_left(pool) && !added)
                {
                    winner = get_player_points(players[0]) < get_player_points(players[1]);
                }
            }
            else
            {
                winner = 1 - turn;
            }
            
            turn = 1 - turn;
        }
        free_tetrimino(selected_t);
    }
    pvp_end_game(winner, gameFields, pool, points, players, start_time, moves);
}

/*da finire di aggiustare*/
void pvp_end_game(int win_flag,gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points, player_t **players,unsigned int start_time,int *moves)/*thanos++*/{
    WINDOW* field1Win = get_gamefield_win(gameFields[0]);
    WINDOW* field2Win = get_gamefield_win(gameFields[1]);
    WINDOW* poolWin = get_pool_win(pool);
    WINDOW* pointWin = get_pointboard_win(points);
    WINDOW *summary;

    unsigned int end_time = (int)time(NULL);
    char ch;
    int i;

    moves[0] = moves[0];
    moves[1] = moves[1];
    char* p1_nickname = get_player_nick(players[0]);
    char* p2_nickname = get_player_nick(players[1]);
    unsigned int playersPoints[2] = { get_player_points(players[0]), get_player_points(players[1])};
    kill_win(field1Win);
    kill_win(field2Win);
    kill_win(poolWin);
    kill_win(pointWin);
    mvprintw(5,0, "                                ");
    refresh();

    summary = newwin( 18, COLS-2, (LINES/2) -5, 1 );
    box(summary, 0, 0 );
    mvwprintw(summary,0,1," GAME OVER ");
    
    wmove(summary,2,2);
    wprint_with_delay(summary,20,PVP_END_TXT[0]);
    wprint_with_delay(summary,20,PVP_END_TXT[1]);
    wprint_with_delay(summary,20,p1_nickname);
    wprint_with_delay(summary,20," : ");
    wprintw(summary,"%05u",playersPoints[0]);
    wmove(summary,6,2);
    wprint_with_delay(summary,20,PVP_END_TXT[1]);
    wprint_with_delay(summary,20,p2_nickname);
    wprint_with_delay(summary,20," : ");
    wprintw(summary,"%05u",playersPoints[1]);
    wrefresh(summary);
    
    delay(500);
    
    wmove(summary,8,2);
    wprint_with_delay(summary,20,PVP_END_TXT[2]);
    
    wprintw(summary,"%05u s",(end_time-start_time));
    wrefresh(summary);

    delay(500);
    
    wmove(summary,10,2);
    wprint_with_delay(summary,20,PVP_END_TXT[3]);
    wprint_with_delay(summary,20,p1_nickname);
    wprint_with_delay(summary,20," :      ");
    
    wprintw(summary,"%05d",moves[0]);
    wrefresh(summary);
    
    wmove(summary,11,2);
    wprint_with_delay(summary,20,PVP_END_TXT[3]);
    wprint_with_delay(summary,20,p2_nickname);
    wprint_with_delay(summary,20," :      ");
    
    wprintw(summary,"%05d",moves[1]);
    wrefresh(summary);

    wmove(summary,13,2);

    wprint_with_delay(summary,20,PVP_END_TXT[4]);

    i=0;

    if(!win_flag){
        wprint_with_delay(summary,20,p1_nickname);
    }
    else if(win_flag){
        wprint_with_delay(summary,20,p2_nickname);
    }
    else{ wprintw(summary,"PAREGGIO"); wrefresh(summary);} /*In realtá non accade mai, almeno, dipende dalle disposizioni*/
    
    delay(500);

    wmove(summary,16,(COLS/2)-9);
    wattron(summary, A_STANDOUT );
    wprintw(summary,"> Torna al menu! <");
    wattroff(summary, A_STANDOUT );
    wrefresh(summary);

    free_player(players[0]);
    free_player(players[1]);
    free(players);
    free_gamefield(gameFields[0]);
    free_gamefield(gameFields[1]);
    free(gameFields);
    free_pool(pool);
    free_pointboard(points);
    free(moves);

    ch = -1;

    do{
        ch = wgetch(summary);
    }while(ch != 10);

    kill_win(summary);
    return;
}