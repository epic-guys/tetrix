#include <ncurses.h>
#include <malloc.h>
#include <string.h>
#include <constants.h>

typedef struct Player
{
    int points;
    char *nickname;
} player_t;

/**
 * @brief Istanzia uno struct
 * giocatore e lo inizializza con il nickname
 * passato e con cursore e punti a 0.
 * @param[in] nick Il nome del giocatore.
 * @return Lo struct giocatore istanziato. 
 */
player_t *initializePlayer(char* nick)
{
    player_t *player = (player_t*) malloc(sizeof(player_t));
    player->points = 0;
    player->nickname = nick;
    return player;
}

typedef struct PointBoard
{
    WINDOW *win;
    player_t *player_1;
    player_t *player_2;

}pointboard_t;

void refreshPointBoard(pointboard_t *board)
{
    mvwprintw(board->win, 2, 2, "%s", board->player_1->nickname);
    mvwprintw(board->win, 2, POINTBOARD_COLS-6, "%05d", board->player_1->points);
    if (board->player_2)
    {
        mvwprintw(board->win, 4, 2, "%s", board->player_2->nickname);
        mvwprintw(board->win, 2, POINTBOARD_COLS-6, "%05d", board->player_2->points);
    }
    wrefresh(board->win);
}

/**
 * @brief Inizializza una finestra che tiene traccia
 * dei punti di ogni giocatore. Viene posizionata con l'angolo
 * in alto a sinistra sulle coordinate specificate.
 * 
 * @param y L'asse y in cui posizionare la point board.
 * @param x L'asse x in cui posizionare la point board.
 * @param player_1 Il giocatore 1.
 * @param player_2 Il giocatore 2. In caso di giocatore singolo deve essere NULL.
 * @return pointboard_t* La point board inizializzata.
 */
pointboard_t *initializePointBoard(int y, int x, player_t *player_1, player_t *player_2)
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
        x
    );
    box(w, 0, 0);
    
    pointBoard->win = w;
    pointBoard->player_1 = player_1;
    pointBoard->player_2 = player_2;
    refreshPointBoard(pointBoard);

    return pointBoard;
}

/**
 * @brief metodo getter per ricevere la finestra ncurses della finestra di punti
 * @param[in] p puntatore alla struct della pointboard
 * @param[out] win la finestra di ncurses del campo da gioco
 */
WINDOW* getPointBoardWin(pointboard_t *p){
    return p->win;
}

/**
 * @brief aggiunge punti al giocatore
 * @param[in] p Struct del giocatore
 * @param[in] board Struct della tabella dei punti
 * @param[in] points Punti da aggiungere
 */
void playerAddPoints(player_t* p,pointboard_t* board,int points){
    p->points+= points;
    refreshPointBoard(board);
}

/**
 * @brief ritorna il nickname di un player
 * @param[in] p Struct del giocatore
 * @param[out] nickname stringa con il nickname del giocatore
 */
char* getPlayerNick(player_t* p){
    return p->nickname;
}

/**
 * @brief ritorna il nickname di un player
 * @param[in] p Struct del giocatore
 * @param[out] points punti del giocatore
 */
int getPlayerPoints(player_t* p){
    return p->points;
}