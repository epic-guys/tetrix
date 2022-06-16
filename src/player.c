#include <ncurses.h>
#include <malloc.h>
#include <string.h>

#include "../include/constants.h"

typedef struct Player
{
    unsigned int points;
    char *nickname;
} player_t;

/**
 * @brief Istanzia uno struct
 * giocatore e lo inizializza con il nickname
 * passato e con cursore e punti a 0.
 *
 * @param[in] nick Il nome del giocatore.
 * @param[out] p Lo struct giocatore istanziato.
 */
player_t *initialize_player(char *nick)
{
    player_t *player = (player_t *)malloc(sizeof(player_t));
    player->points = 0;
    player->nickname = nick;

    refresh();
    return player;
}

/**
 * @brief free del player.
 *
 * @param[in] p il player.
 */
void free_player(player_t *p)
{
    free(p->nickname);
    free(p);
}

typedef struct PointBoard
{
    WINDOW *win;
    player_t *player_1;
    player_t *player_2;
} pointboard_t;

void refresh_pointboard(pointboard_t *board);

/**
 * @brief Inizializza una finestra che tiene traccia
 * dei punti di ogni giocatore. Viene posizionata con l'angolo
 * in alto a sinistra sulle coordinate specificate.
 *
 * @param[in] y L'asse y in cui posizionare la point board.
 * @param[in] x L'asse x in cui posizionare la point board.
 * @param[in] player_1 Il giocatore 1.
 * @param[in] player_2 Il giocatore 2. In caso di giocatore singolo deve essere NULL.
 * @param[out] pointboard_t La point board inizializzata.
 */
pointboard_t *initialize_pointboard(int y, int x, player_t *player_1, player_t *player_2)
{
    pointboard_t *pointBoard = (pointboard_t *)malloc(sizeof(pointboard_t));

    WINDOW *w;
    /*
    Se c'è un giocatore la point board è alta 5.
    Se ci sono due giocatori la point board è alta 7.
    */
    w = newwin(
        player_2 ? POINTBOARD_ROWS_2P : POINTBOARD_ROWS_1P,
        POINTBOARD_COLS,
        y,
        x);
    box(w, 0, 0);

    pointBoard->win = w;
    pointBoard->player_1 = player_1;
    pointBoard->player_2 = player_2;
    refresh_pointboard(pointBoard);

    return pointBoard;
}

/**
 * @brief free della pointboard.
 *
 * @param[in] p La pointboard.
 */
void free_pointboard(pointboard_t *p)
{
    free(p);
}

/**
 * @brief ricarica la pointboard
 *
 * @param[in] board La pointboard da ricaricare.
 */
void refresh_pointboard(pointboard_t *board)
{

    mvwprintw(board->win, 2, 1, "%s", board->player_1->nickname);
    mvwprintw(board->win, 2, POINTBOARD_COLS - 6, "%05d", board->player_1->points);
    if (board->player_2)
    {
        mvwprintw(board->win, 4, 1, "%s", board->player_2->nickname);
        mvwprintw(board->win, 4, POINTBOARD_COLS - 6, "%05d", board->player_2->points);
    }
    wrefresh(board->win);
}

/**
 * @brief metodo getter per ricevere la finestra ncurses della finestra di punti.
 *
 * @param[in] p puntatore alla struct della pointboard.
 * @param[out] win la finestra di ncurses del campo da gioco.
 */
WINDOW *get_pointboard_win(pointboard_t *p)
{
    return p->win;
}

/**
 * @brief aggiunge punti al giocatore.
 *
 * @param[in] p Struct del giocatore.
 * @param[in] board Struct della tabella dei punti.
 * @param[in] points Punti da aggiungere.
 */
void player_add_points(player_t *p, pointboard_t *board, int points)
{
    p->points += points;
    refresh_pointboard(board);
}

/**
 * @brief ritorna il nickname di un player.
 *
 * @param[in] p Struct del giocatore.
 * @param[out] nickname stringa con il nickname del giocatore.
 */
char *get_player_nick(player_t *p)
{
    return p->nickname;
}

/**
 * @brief ritorna i punti di un player.
 *
 * @param[in] p Struct del giocatore.
 * @param[out] points punti del giocatore.
 */
unsigned int get_player_points(player_t *p)
{
    return p->points;
}

/**
 * @brief Restituisce il numero di punti corrispondenti
 * alle righe riempite.
 *
 * @param deletedRows Le righe riempite.
 * @return I punti.
 */
int get_points(int deletedRows)
{
    switch (deletedRows)
    {
    case 1:
        return POINTS_ONE_ROW_DELETED;
    case 2:
        return POINTS_TWO_ROW_DELETED;
    case 3:
        return POINTS_THREE_ROW_DELETED;
    case 4:
        return POINTS_FOUR_ROW_DELETED;
    default:
        return 0;
    }
}
