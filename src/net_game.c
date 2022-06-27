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
void new_net_game(char* srv_nick, char* client_nick,int is_server,int socket,int turn);
void net_continue_game(player_t **players, gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points, int is_server,int socket,int turn);

/**
 * @brief stampa le istruzioni per la partita in LAN.
 *
 * @param[in] nickname puntatore al nome del giocatore.
 */
void LAN_instructions(char *nickname)
{
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
        /*Creo il sock, mostro l'ip (?)*/
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
 * @param[in] is_server 1 se è per il sock, in tal caso visualizzerà anche
 * il pulsante "Gioca". 0 se è un client, non mostrerà il pulsante.
 * @return WINDOW* 
 */
WINDOW* init_lobby_menu(char* srv_nickick,char* clt_nick, int is_server)
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
    return win;
}

#pragma region SERVER

void new_game_host(char* nickname)
{
    WINDOW* win = newwin(10, COLS - 2, LINES / 2, 1);
    char* clt_nick;
    char ch = -1;
    int sock;
    int turn;
    box(win, 0, 0);
    wmove(win, getmaxy(win) / 2, getmaxx(win) / 2 - 27);
    wprintw(win,"Il tuo ip: <Non Implementato> ");
    wprintw(win, "In attesa di connessione...");
    wrefresh(win);

    /* ATTENZIONE, BLOCCANTE */
    sock = host_game();

    /* SCAMBIO DI NICKNAME */
    clt_nick = recv_nickname(sock);
    send_nickname(sock, nickname);

    kill_win(win);
    refresh();
    win = init_lobby_menu(nickname, clt_nick, 1);
    while (ch != '\n')
    {
        ch = getch();
    }
    turn = 0 /* rand() % 2 */;
    send_start_game(sock,turn);
    kill_win(win);
    new_net_game(nickname,clt_nick,1,sock,turn);
    
}

void new_net_game(char* srv_nick, char* client_nick,int is_server,int socket,int turn)
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

    refresh();

    net_continue_game(players, gameFields,pool,points,is_server,socket, turn);
}

#pragma endregion


#pragma region CLIENT

void new_game_client(char* nickname, char* ip)
{
    int turn;
    WINDOW* win;
    int socket = connect_to_game(ip);
    char* srv_nick;
    send_nickname(socket, nickname);
    srv_nick = recv_nickname(socket);

    win = init_lobby_menu(srv_nick, nickname, 0);

    turn = recv_start_game(socket);
    kill_win(win);
    refresh();
    new_net_game(srv_nick,nickname,0,socket,turn);
}

#pragma endregion

#pragma region COMMON

void net_continue_game(player_t **players, gamefield_t **gameFields, tetrimini_pool_t *pool, pointboard_t *points, int is_server,int socket,int turn)
{
    tetrimino_type_t selected_i;
    int winner = -1;
    tetrimino_t *selected_t;
    

    while(winner<0)
    {
        int cursor = -1, added, deletedRows;
        if (turn == !is_server)
        {
            /* gioca server */
            selected_i = select_tetrimino(pool);
            selected_t = get_tetrimino(selected_i);
            cursor = (FIELD_COLS - get_tet_cols(selected_t)) / 2;
            
            refresh_selector(gameFields[turn], selected_t, cursor);

            /*            
            while (cursor < 0)
            */
            cursor = manage_drop(gameFields[turn], selected_t);

            added = add_tetrimino_to_gamefield(gameFields[turn], selected_t, cursor);

            send_field(socket,get_gamefield(gameFields[turn]));
            send_tet_type(socket,selected_i);
            send_added_tet(socket, added);
            free_tetrimino(selected_t);
        }
        else
        {
            int *tmp = get_gamefield(gameFields[turn]);
            change_field(&tmp, recv_field(socket));
            selected_i = recv_tet_type(socket);
            added = 1; recv_added_tet(socket);
        }

        remove_tetrimino_from_pool(selected_i, pool);

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
            if (no_tetriminos_left(pool))
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
    /*
    Qualcuno ha vinto
    */
}

#pragma endregion