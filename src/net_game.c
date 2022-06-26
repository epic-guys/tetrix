#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include "../include/player.h"
#include "../include/gamefield.h"
#include "../include/tetrimino.h"
#include "../include/functions.h"
#include "../include/constants.h"
#include "../include/net_game.h"
#include "../include/networking.h"

void new_game_host(char* nickname);
void new_game_client(char* nickname, char* ip);

/**
 * @brief stampa le istruzioni per la partita in LAN.
 *
 * @param[in] nickname puntatore al nome del giocatore.
 */
void LAN_instructions(char *nickname)
{
    WINDOW *instructions_win;
    char ch;
    int art_cols = get_ASCII_art_rows(ART_LOGO);
    /* 3 è l'altezza della finestra dei credits */
    instructions_win = newwin(LINES - art_cols - 3, COLS, 6, 0);
    box(instructions_win, 0, 0);
    mvwprintw(instructions_win, 0, 1, " BENVENUTO ");

    wmove(instructions_win, 2, 2);
    wprint_with_delay(instructions_win, TXT_DELAY, SINGLE_WELCOME_TXT[0]);
    wprint_with_delay(instructions_win, TXT_DELAY, nickname);
    wmove(instructions_win, 3, 2);
    wprint_with_delay(instructions_win, TXT_DELAY, SINGLE_WELCOME_TXT[1]);

    delay(1000);

    wmove(instructions_win, 18, (COLS / 2) - 4);
    wattron(instructions_win, A_STANDOUT);
    wprintw(instructions_win, "> Gioca! <");
    wattroff(instructions_win, A_STANDOUT);
    wrefresh(instructions_win);

    ch = -1;

    do
    {
        ch = wgetch(instructions_win);
    } while (ch != 10);

    kill_win(instructions_win);
    return;
}

void connection_menu(char* nickname)
{
    char* ip;
    WINDOW *w;
    int N_items = 2;
    char list[2][15] = {"Crea Partita", "Connettiti"};
    char item[15];
    int ch = -1, i = 0, width = 7;
    w = newwin(10, COLS - 2, LINES / 2, 1);
    box(w, 0, 0);

    for (i = 0; i < N_items; ++i)
    {
        if (i == 0)
            wattron(w, A_STANDOUT);
        else
            wattroff(w, A_STANDOUT);
        sprintf(item, "%-7s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);
    }

    /*carica lo schermo*/
    wrefresh(w);
    i = 0;
    /*sposta il focus della tastiera sulla finestra*/
    keypad(w, TRUE);
    /* Nasconde il cursore di sistema*/
    curs_set(0);

    do
    {
        ch = wgetch(w);

        sprintf(item, "%-7s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);

        switch (ch)
        {
        case KEY_UP:
            i--;
            i = (i < 0) ? N_items - 1 : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > N_items - 1) ? 0 : i;
            break;
        }

        /*Sottolinea la scelta*/
        wattron(w, A_STANDOUT);
        sprintf(item, "%-7s", list[i]);
        mvwprintw(w, i + 1, 2, "%s", item);
        wattroff(w, A_STANDOUT);

    } while (ch != 10);

    switch (i)
    {
    case 0:
        /*Creo il server, mostro l'ip (?)*/
        kill_win(w);
        new_game_host(nickname);
        break;
    case 1:
        /*chiedo solo un IP, per favore*/
        kill_win(w);
        
        do
        {
            ip = form(16, " IPv4");
            
        } while (!is_an_ip(ip));
        
        new_game_client(nickname, ip);
        break;
    default:
        delwin(w);
        endwin();
        printf("Tetrix ha Flatlineato"); /* Grande citazione */
        break;
    }
}

/**
 * @brief inizia una partita in single player
 */
void net_new_game()
{


    char *nickname;
    nickname = form(16, " Nome: ");

    #ifndef DEBUG
    LAN_instructions(nickname);
    #endif

    connection_menu(nickname);

    return;
}

/**
 * @brief Crea il menu della lobby.
 * 
 * @param[in] srv_nickick Il nickname dserverore.
 * @param[in] clt_nick Il nickname del client.
 * @param[in] is_server 1 se è per il server, in tal caso visualizzerà anche
 * il pulsante "Gioca". 0 se è un client, non mostrerà il pulsante.
 * @return WINDOW* 
 */
WINDOW* init_lobby_menu(char* srv_nickick, chaclt_nickck2, int is_server)
{
    WINDOW* win = newwin(20, 20, (LINES / 2) - 10, 1);
    box(win, 0, 0);
    mvwprintw(win, 2, 2, srv_nickick);
    mvwprintw(win, 4, 2, clt_nick);
    if (is_server)
    {
        wattron(win, A_STANDOUT);
        mvwprintw(win, getmaxy(win) - 2, getmaxx(win) / 2 - 4, "> GIOCA <");
        wattroff(win, A_STANDOUT);
    }
    wrefresh(win);
}

#pragma region SERVER

void new_game_host(char* nickname)
{
    WINDOW* win = newwin(10, COLS - 2, LINES / 2, 1);
    char* clt_nick;
    char ch = -1;
    box(win, 0, 0);
    wmove(win, getmaxy(win) / 2, getmaxx(win) / 2 - 27);
    wprintw(win,"Il tuo ip: <Non Implementato> ");
    wprintw(win, "In attesa di connessione...");
    wrefresh(win);

    /* ATTENZIONE, BLOCCANTE */
    srvconf_t server = host_game();

    /* SCAMBIO DI NICKNAME */
    clt_nick = recv_nickname(server.conn_socket);
    send_nickname(server.conn_socket, nickname);

    kill_win(win);
    win = init_lobby_menu(nickname, clt_nick, 1);
    while (ch != '\n')
    {
        ch = getch();
    }
    kill_win(win);
    new_net_game(nickname,clt_nick,0,socket);
    
}

void new_net_game(char* srv_nick, char* client_nick,int is_server,int socket)
{
    player_t **players = (player_t **)malloc(sizeof(player_t *) * 2);
    gamefield_t **gameFields = (gamefield_t **)malloc(sizeof(gamefield_t *) * 2);

    tetrimini_pool_t *pool;
    pointboard_t *points;

    players[0] = initialize_player(srv_nick);
    players[1] = initialize_player(client_nick);

    gameFields[0] = initialize_gamefield(7, 5);
    gameFields[1] = initialize_gamefield(7, (COLS / 2) + (POOL_COLS / 2) + 5);
    pool = initialize_pool(6, (COLS / 2) - (POOL_COLS / 2) - 3);
    points = initialize_pointboard(0, COLS - 30, players[0], players[1]);

    net_continue_game(players, gameFields,pool,points,is_server,socket);
}

#pragma endregion


#pragma region CLIENT

void new_game_client(char* nickname, char* ip)
{
    WINDOW* win;
    int socket = connect_to_game(ip);
    char* srv_nick;
    send_nickname(socket, nickname);
    srv_nick = recv_nickname(socket);

    win = init_lobby_menu(srv_nick, nickname, 0);
    
    

    if(recv_start_game(socket)){
        kill_win(win);
        new_net_game(srv_nick,nickname,1,socket);
    }
}

#pragma endregion

#pragma region COMMON

void net_continue_game(player_t **players, gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points, int is_server,int socket)
{
    tetrimino_type_t selected_i;
    int winner = -1;
    tetrimino_t *selected_t;
    int turn;
    turn = rand() % 2;

    while(winner<0)
    {
        int cursor;
        if(turn == 0)
        {
            /*gioca il server*/
            if(is_server)
            {
                selected_i = select_tetrimino(pool);
                selected_t = get_tetrimino(selected_i);
                cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;
                
                refresh_selector(gameFields[turn], selected_t, cursor);
                cursor = manage_drop(gameFields[turn], selected_t);
                send_field(socket,g->field);
            }
            else{
                change_field(&get_field(gameFields[turn]),recv_field(socket));
            }
            
        }
        else
        {
            /*gioca il client*/
        }
    }
}

#pragma endregion