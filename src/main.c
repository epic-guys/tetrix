#include <stdio.h>
#include <ncurses.h>

#include "../include/singleplayer.h"
#include "../include/multiplayer.h"
#include "../include/functions.h"
#include "../include/constants.h"

void init_colors();
void init_main_screen();
void print_logo();
void print_menu();
void print_credits();

/**
 * @brief inizializza la finestra main di ncurses con i nostri parametri (é una funzione stub)
 */
void init_main_screen()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    init_colors();
    refresh();
}
/**
 * @brief inizializziamo tutti i colori (é una funzione stub)
 */
void init_colors()
{
    start_color();
    /* CIANO su sfondo NERO */
    init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);

    /* BLU su sfondo NERO */
    init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);

    /* ARANCIONE su sfondo NERO */
    init_color(8, 1000, 647, 0);
    init_pair(PAIR_ORANGE, COLOR_ORANGE, COLOR_BLACK);

    /* GIALLO su sfondo NERO */
    init_pair(PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);

    /*VERDE su sfondo NERO*/
    init_pair(PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);

    /*VIOLA su sfondo NERO*/
    init_color(9, 218, 112, 214);
    init_pair(PAIR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);

    /*ROSSO su sfondo NERO*/
    init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);

    /*sfondo OMBREGGIATURA*/
    /*init_pair(100,COLOR_BLACK,COLOR_MAGENTA);*/

    /* NERO su sfondo CIANO */
    init_pair(PAIR_BG_CYAN, COLOR_BLACK, COLOR_CYAN);

    /* NERO su sfondo BLU */
    init_pair(PAIR_BG_BLUE, COLOR_BLACK, COLOR_BLUE);

    /* NERO su sfondo ARANCIONE */
    init_pair(PAIR_BG_ORANGE, COLOR_BLACK, COLOR_ORANGE);

    /* NERO su sfondo GIALLO */
    init_pair(PAIR_BG_YELLOW, COLOR_BLACK, COLOR_YELLOW);

    /*NERO su sfondo VERDE*/
    init_pair(PAIR_BG_GREEN, COLOR_BLACK, COLOR_GREEN);

    /*NERO su sfondo VIOLA*/
    init_pair(PAIR_BG_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);

    /*NERO su sfondo ROSSO*/
    init_pair(PAIR_BG_RED, COLOR_BLACK, COLOR_RED);
}

/**
 * @brief stampa il logo del gioco in ASCII art.
 */
void print_logo()
{
    WINDOW *logo;
    int i, j, space;

    logo = newwin(10, COLS - 2, 0, 0);

    space = (COLS - get_ASCII_art_cols(ART_LOGO)) / 2 - 5;
    mvwprintwrows(logo, 0, space, ART_LOGO);
    wrefresh(logo);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
}

/**
 * @brief stampa la finestra in basso con i nostri crediti.
 */
void print_credits()
{
    WINDOW *credits;
    int x, y;
    credits = newwin(3, COLS - 2, LINES - 3, 1);
    box(credits, 0, 0);
    wmove(credits, getcurx(credits) + 1, getcury(credits) + 2);
    wprintw(credits, "PROGETTO DI GRUPPO - 2022 - DAVIDE ANTONIO AMODIO - LUCA BOFFO - ALVISE FAVERO");
    wrefresh(credits);
}

/**
 * @brief stampa e gestisce il menú principale.
 */
void main_menu()
{
    WINDOW *w;
    /*Quanti elementi ci sono nel menú*/
    int N_items = 4;
    char list[4][15] = {"Single Player", "PvP", "PvCPU", "Quit"};
    char item[15];
    int ch, i = 0, width = 7;
    /* Crea una nuova finestra */
    w = newwin(10, COLS - 2, LINES / 2, 1);
    /* imposta i bordi*/
    box(w, 0, 0);

    /* Stampa il menu iniziale*/
    for (i = 0; i < N_items; ++i)
    {
        if (i == 0)
            /*"sottolinea" il primo elemento*/
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

    } while (ch != 10 /* significa enter (non so nemmeno io perché ma funzia) */);

    switch (i)
    {
    case 0:
        /* Single */
        kill_win(w);
        new_game_single();
        main_menu();
        break;
    case 1:
        /* PvP */
        kill_win(w);
        pvp_new_game();
        main_menu();
        break;
    case 2:
        /* PvCPU */
        kill_win(w);
        pve_new_game();
        main_menu();
        break;
    case 3:
        endwin();
        break;

    default:
        delwin(w);
        endwin();
        printf("Tetrix ha Flatlineato");
        break;
    }
}

int main()
{
    /*Inizia il programma*/
    /*Attivo NCURSES con i nostri parametri*/
    init_main_screen();
    if (LINES < 30 || COLS < 125)
    {
        endwin();
        printf("ERRORE! \nIl programma richiede almeno 125 colonne e 30 righe\n");
        return 69;
    }
    print_credits();
    print_logo();
    main_menu();
    return 0;
}