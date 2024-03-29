#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../include/tetrimino.h"
#include "../include/constants.h"

void kill_win(WINDOW *w);

/**
 * @brief Funzione per creare un form al centro dello schermo
 * la risposta viene caricata in un array di char
 * ATTENZIONE: la memoria viene allocata all'interno della funzione!
 *
 * @param[in] m_size dimensione massima della risposta che si vuole ricevere.
 * @param[in] title titolo della finestra.
 * 
 * @return L'array contenente la stringa, di dimensione m_size + 1.
 */
char *form(int m_size, char title[])
{
    int i, ch, j, flag = 0;
    WINDOW *w;
    char *memory = calloc(m_size + 1, sizeof(char));

    m_size--;

    w = newwin(3, (strlen(title) > m_size ? strlen(title) : m_size) + 6, (LINES / 2) - 1, (COLS / 2) - (((strlen(title) > m_size ? strlen(title) : m_size) / 2) + 3));
    box(w, 0, 0);

    mvwprintw(w, 0, 1, "%s", title);

    /*stampa gli underscore, mette il cursore, pulisce la memoria, aggiunge il null terminator*/
    for (i = 0; i < m_size; ++i)
    {
        if (i == 0)
        {
            wattron(w, A_STANDOUT);
        }
        else
        {
            wattroff(w, A_STANDOUT);
        }
        mvwprintw(w, 1, 2 + i, "_");
    }

    wrefresh(w);

    i = 0;
    /*
    Bisogna fare un loop infinito perché l'input viene preso all'interno della funzione.
    Sarà poi la funzione a doverlo rompere appena incontra il carattere '\n'.
    Utilizzando il metodo vecchio, viene aggiunto il carattere '\n' alla stringa.
    */
    while (1)
    {
        ch = wgetch(w);
        /*
        Avrei voluto usare uno switch ma sarebbe stato un problema fare break,
        dato che è necessario per lo switch.
        */
        if (ch == '\n')
            break;
        else if (ch == 127 || ch == KEY_BACKSPACE)
        {
            if (i > 0)
            {
                memory[--i] = '\0';
            }
        }
        else
        {
            if (i < m_size)
            {
                memory[i++] = ch;
            }
        }

        for (j = 0; j < m_size; ++j)
        {
            if (memory[j] == '\0')
            {
                if (flag == 0)
                {
                    wattron(w, A_STANDOUT);
                    flag = 1;
                }
                else
                {
                    wattroff(w, A_STANDOUT);
                }
                mvwprintw(w, 1, 2 + j, "_");
            }

            else
                mvwprintw(w, 1, 2 + j, "%c", memory[j]);
        }
        flag = 0;
        wrefresh(w);
    }

    kill_win(w);

    memory[++i] = '\0';

    return memory;
}

/**
 * @brief Conta il numero di righe di una
 * ASCII art rettangolare importata in C generando i bytes con xxd.
 *
 * @param[in] ascii La stringa contenente la ASCII art.
 * 
 * @return Il numero di righe.
 */
int get_ASCII_art_rows(const unsigned char ascii[])
{
    int i = 0, r = 1;
    while (ascii[i] != '\0')
    {
        if (ascii[i] == '\n')
        {
            ++r;
        }
        ++i;
    }
    return r;
}

/**
 * @brief Conta il numero di colonne di una
 * ASCII art rettangolare importata in C generando i bytes con xxd.
 * 
 * @param[in] ascii La stringa contenente la ASCII art.
 * 
 * @return Il numero di colonne
 */
int get_ASCII_art_cols(const unsigned char ascii[])
{
    int i = 0;
    while (ascii[i] != '\n' && ascii[i] != '\0')
        ++i;
    return i;
}

/**
 * @brief una funzione delay. [ATTENZIONE, É BLOCCANTE]
 *
 * @param[in] millisec tempo in millisecondi.
 */
void delay(int millisec)
{
    if (millisec == 0)
        return;
    millisec = millisec * CLOCKS_PER_SEC / 1000;
    clock_t start_time = clock();
    while (clock() < start_time + millisec)
        ;
}

/**
 * @brief elimina le finestre di ncurses.
 *
 * @param[in] w la finestra da eliminare.
 */
void kill_win(WINDOW *w)
{
    wclear(w);
    wrefresh(w);
    delwin(w);
}

/**
 * @brief stampa una riga di testo carattere per carattere con un delay fra ogni lettera.
 *
 * @param[in] w la finestra dove stampare.
 * @param[in] d il delay fra o caratteri.
 * @param[in] c la stringa di caratteri da stampare.
 */
void wprint_with_delay(WINDOW *w, int d, const char *c)
{
    int i = 0;
    char ch;
    while (c[i] != '\0')
    {
        if (c[i] != '\n')
            wprintw(w, "%c", c[i]);
        else
            wmove(w, getcury(w) + 1, 2);
        i++;
        wrefresh(w);
        delay(d);
    }
}

/**
 * @brief stampa una riga di testo carattere per carattere peró supporta anche il \n come a capo.
 * 
 * @param[in] w la finestra dove stampare.
 * @param[in] y la riga dove stampare.
 * @param[in] x la colonna dove stampare.
 * @param[in] c il puntatore alla stringa di caratteri da stampare.
 */
void mvwprintwrows(WINDOW *w, int y, int x, const char *c)
{
    int i = 0, j = 0;
    wmove(w, y, x);
    while (c[j] != '\0')
    {
        if (c[j] == '\n')
        {
            wmove(w, y + ++i, x);
        }
        else
        {
            waddch(w, c[j]);
        }
        ++j;
    }
}

/**
 * @brief stampa una riga di testo carattere per carattere peró supporta anche il \n come a capo.
 * 
 * @param[in] w la finestra dove stampare.
 * @param[in] y la riga dove stampare.
 * @param[in] x la colonna dove stampare.
 * @param[in] c il puntatore alla stringa di caratteri da stampare.
 * @param[in] start da dove colorare.
 * @param[in] stop da dove smettere di colorare.
 * @param[in] cols da quante colonne é composta la stringa.
 */
void mvwprintwrows_with_rainbow(WINDOW *w, int y, int x, const char *c,int start,int stop,int cols)
{
    int i = 0, j = 0,k=0;
    wmove(w, y, x);
    while (c[j] != '\0')
    {
        if (c[j] == '\n')
        {
            wmove(w, y + ++i, x);
            k=0;
        }
        else
        {
            k++;
            int colored,color = random_color();
            if(k>=start && k<=stop)
            {
                colored = 1;
                wattron(w, COLOR_PAIR(color));
            }
            waddch(w, c[j]);
            if(colored)
            {
                wattroff(w, COLOR_PAIR(color));
            }
        }
        ++j;
    }
}

/**
 * @brief scambia il foreground color con il background color e viceversa.
 *
 * [Schema di funzionamento]
 * Nel file constants.h abbiamo definito i colori e i pair [colore, testo] identificati da un numero
 * successivamente sono stati creati i pair invertiti assegnando il valore numerico del pair + 100
 * quindi dato il pair [n], [n+100] sará lo stesso pair che peró avrá il colore del testo come sfondo e viceversa
 * Automaticamente facendo l'operazione inversa su un pair n t.c n > 100
 * trasformeremo un colore di sfondo in un colore del testo.
 *
 * @param[in] c colore sotto forma numerica.
 * 
 * @return c colore sotto forma numerica.
 */
int swap_color(int c)
{
    if (c < 100)
        return c + 100;
    return c - 100;
}

/**
 * @brief ritorna un colore a caso tra quelli definiti in constants.h.
 *
 * @return int il colore sotto forma numerica.
 */
int random_color()
{
    return rand() % (N_tetrimini /*numero colori*/) + 1;
}

/**
 * @brief Restituisce l'indice dell'elemento successivo
 * in una struttura dati ordinata, ipotizzando che questa sia circolare.
 * Se i è l'ultimo elemento, allora restituisce il primo.
 *
 * @param[in] i L'indice attuale.
 * @param[in] max La dimensione della struttura dati.
 * 
 * @return L'indice dell'elemento successivo.
 */
int next_circular(int i, int size)
{
    if (i >= size - 1)
        return 0;
    else
        return i + 1;
}

/**
 * @brief Restituisce l'indice dell'elemento precedente
 * in una struttura dati ordinata, ipotizzando che questa sia circolare.
 * Se i è il primo elemento, allora restituisce l'ultimo.
 *
 * @param[in] i L'indice attuale.
 * @param[in] max La dimensione della struttura dati.
 * 
 * @return L'indice dell'elemento successivo.
 */
int prev_circular(int i, int size)
{
    if (i <= 0)
        return size - 1;
    else
        return i - 1;
}

/**
 * @brief funzione che serve per comparare due campi da gioco.
 * 
 * @param[in] first il puntatore al primo campo da gioco.
 * @param[in] second il puntatore al secondo campo da gioco.
 * 
 * @return la prima riga diversa.
 */
int compare_fields(int *first, int *second)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; i++)
    {
        for (j = 0; j < FIELD_COLS; j++)
        {
            if (first[i * FIELD_COLS + j] != second[i * FIELD_COLS + j])
            {
                return i;
            }
        }
    }
    return FIELD_ROWS;
}

/**
 * @brief Controlla se una matrice qualsiasi contiene celle vuote bloccate
 * da celle piene al di sopra.
 * @param[in] mat La matrice da controllare.
 * @param[in] rows Il numero di righe della matrice.
 * @param[in] cols Il numero di colonne della matrice.
 * 
 * @return 0 se la matrice non ha celle bloccate, altrimenti restituisce
 * $ r \times c $, con $r$ il numero di righe bloccate, $c$ le colonne bloccate.
 */
int blank_cells(int* mat, int rows, int cols)
{
    int i, j;
    int max_rows = 0, max_cols = 0;

    for (j = cols - 1; j >= 0; --j)
    {
        int found_free = 0;
        int stuck_rows = 0;
        for (i = rows - 1; i >= 0; --i)
        {
            if (mat[cols * i + j] == 0)
            {
                found_free = 1;
            }
            else
            {
                if (found_free)
                {
                    stuck_rows = rows - i - 1;
                }
            }
        }

        if (stuck_rows > 0)
        {
            ++max_cols;
            if (stuck_rows > max_rows) max_rows = stuck_rows;
        }
    }

    return max_rows * max_cols;
}
