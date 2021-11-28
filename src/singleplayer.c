#include <ncurses.h>
#include "gamefield.c"
#include "player.c"
#include "tetrimino.c"

/**
 * @brief inizia una partita in single player
 */
void newGameSingle(){

}


/**
 * @brief Funzione WIP(Work In Progress) che aggiorna la posizione del
 * tetramino sulla zona di posizionamento.
 * Più avanti dovrà ricevere come parametro un tetramino
 * e dovrà rispettare la sua dimensione nel campo da gioco. 
 * 
 * @param[in, out] p Il giocatore in cui aggiornare la posizione.
 * TODO param tetrimino
 */
void refreshSelector(Player *p, Tetrimino t)
{
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        wmove(p->win, i, 0);
        wclrtoeol(p->win);
    }

    for (i = 0; i < t.rows; ++i)
    {
        for (j = 0; j < t.cols; ++j)
        {
            if (t.values[t.cols * i + j])
            {
                wattron(p->win, COLOR_PAIR(p->field[i][j]));
                mvwprintw(p->win, 4 - t.rows + i, (p->cursor_pos + j) * 2, "[]");
                wattroff(p->win, COLOR_PAIR(p->field[i][j]));
            }
        }
    }

    wrefresh(p->win);
}