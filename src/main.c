#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#include <colors.h>

#include "singleplayer.h"

const unsigned char ASCII_logo[] = {
    0x20, 0x20, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6b,
    0x38, 0x62, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0a, 0x20, 0x23, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x20, 0x20, 0x20, 0x2e,
    0x64, 0x24, 0x24, 0x62, 0x2e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64,
    0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x59, 0x38, 0x50, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x41, 0x23, 0x20, 0x20, 0x20, 0x23,
    0x44, 0x23, 0x0a, 0x23, 0x23, 0x23, 0x23, 0x20, 0x20, 0x23, 0x23, 0x23,
    0x20, 0x20, 0x20, 0x64, 0x23, 0x20, 0x20, 0x20, 0x20, 0x2d, 0x62, 0x20,
    0x20, 0x23, 0x23, 0x23, 0x24, 0x24, 0x23, 0x23, 0x23, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x59, 0x23, 0x2e, 0x24, 0x24, 0x24, 0x2c, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x23, 0x2b, 0x23, 0x20, 0x23, 0x2b, 0x23, 0x20, 0x0a, 0x20, 0x20, 0x23,
    0x20, 0x20, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x38, 0x24, 0x2f,
    0x24, 0x5c, 0x24, 0x59, 0x20, 0x20, 0x20, 0x20, 0x20, 0x24, 0x24, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x23, 0x2e, 0x20,
    0x2c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x23, 0x24, 0x23, 0x20, 0x20, 0x20,
    0x23, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x23, 0x2b, 0x3d, 0x2b, 0x23,
    0x20, 0x20, 0x0a, 0x20, 0x23, 0x20, 0x20, 0x23, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x24, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x24, 0x24, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x24, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23,
    0x24, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x23, 0x2b, 0x23, 0x20, 0x23, 0x2b, 0x23, 0x20, 0x0a, 0x23, 0x23, 0x23,
    0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x24, 0x24,
    0x24, 0x24, 0x24, 0x20, 0x20, 0x20, 0x20, 0x22, 0x24, 0x24, 0x3f, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x24, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x23, 0x24, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x4c, 0x23, 0x20, 0x20, 0x20, 0x23,
    0x2a, 0x23};

void initColors();
void initMainScreen();
int getSplashLogoHeight();
int getSplashLogoWidth();
void printLogo();
void printMenu();
void printCredits();

/**
 * @brief inizializza la finestra main di ncurses con i nostri parametri (é una funzione stub)
 */
void initMainScreen(){
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    initColors();
    refresh();
}
/**
 * @brief inizializziamo tutti i colori (é una funzione stub)
 */
void initColors(){
    start_color();
    /* CIANO su sfondo NERO */
    init_pair(PAIR_CYAN,COLOR_CYAN,COLOR_BLACK);
   
   /* BLU su sfondo NERO */
    init_pair(PAIR_BLUE,COLOR_BLUE,COLOR_BLACK);
   
   /* ARANCIONE su sfondo NERO */
    init_color(8,255,240,0);
    init_pair(PAIR_ORANGE,8,COLOR_BLACK);
   
   /* GIALLO su sfondo NERO */
    init_pair(PAIR_YELLOW,COLOR_YELLOW,COLOR_BLACK);
   

   /*VERDE su sfondo NERO*/
    init_pair(PAIR_GREEN,COLOR_GREEN,COLOR_BLACK);
   
   /*VIOLA su sfondo NERO*/
    init_color(9,218,112,214);
    init_pair(PAIR_PURPLE,9,COLOR_BLACK);
   
   /*ROSSO su sfondo NERO*/
    init_pair(PAIR_RED,COLOR_RED,COLOR_BLACK);
}

/**
 * @brief Calcola quante righe occuperà
 * il logo per essere stampato
 * utilizzato per cambiare
 * facilmente il logo senza hardcodare
 * l'altezza del file nel codice
 */
int getSplashLogoHeight()
{
    int i, c = 1;
    for (i = 0; i < sizeof ASCII_logo; ++i)
    {
        if (ASCII_logo[i] == '\n')
        {
            c++;
        }
    }
    return c;
}

/**
 * @brief calcola quante colonne occuperà
 * il logo per essere stampato
 * utilizzato per cambiare
 * facilmente il logo senza hardcodare
 * la larghezza del file nel codice
 */
int getSplashLogoWidth()
{
    int i, c = 1;
    for (i = 0; i < sizeof ASCII_logo; ++i)
    {
        if (ASCII_logo[i] == '\n')
        {
            return c;
        }
        c++;
    }
    return c;
}

void printLogo(){
    WINDOW *logo;
    int i, j, space;
    
    logo = newwin( 10, COLS-2, 1, 1 );

    space = (COLS - getSplashLogoWidth()) / 2 - 5;
    for (j = 0; j < space; ++j)
    {
        wprintw(logo," ");
    }
    for (i = 0; i < sizeof ASCII_logo; i++)
    {
        wprintw(logo,"%c", ASCII_logo[i]);
        if (ASCII_logo[i] == '\n')
        {
            for (j = 0; j < space; ++j)
            {
                wprintw(logo," ");
            }
        }
    }
    wrefresh(logo);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
}

void printCredits(){
    WINDOW *credits;
    int x,y;
    credits = newwin( 3, COLS-2, LINES-3 , 1 );
    box(credits, 0, 0 );
    wmove(credits,getcurx(credits)+1,getcury(credits)+2);
    wprintw(credits,"PROGETTO DI GRUPPO - 2022 - DAVIDE ANTONIO AMODIO - LUCA BOFFO - ALVISE FAVERO");
    wrefresh(credits);
}

void mainMenu(){
    WINDOW *w;
    /*Quanti elementi ci sono nel menú*/
    int N_items = 4;
    char list[4][15] = { "Single Player", "PvP", "PvCPU", "Quit"};
    char item[15];
    int ch, i = 0, width = 7;
    /* Crea una nuova finestra */
    w = newwin( 10, COLS-2, LINES/2 , 1 );
    /* imposta i bordi*/
    box( w, 0, 0 );

    /* Stampa il menu iniziale*/
    for(i=0;i<N_items;++i) {
        if(i == 0)
            /*"sottolinea" il primo elemento*/
            wattron( w, A_STANDOUT );
        else
            wattroff( w, A_STANDOUT );
        sprintf(item, "%-7s",  list[i]);
        mvwprintw( w, i+1, 2, "%s", item );
    }

    /*carica lo schermo*/
    wrefresh(w);
    i = 0;
    /*sposta il focus della tastiera sulla finestra*/
    keypad(w,TRUE);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
    
    do{
        ch = wgetch(w); 
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-7s",  list[i]); 
        mvwprintw( w, i+1, 2, "%s", item ); 
        // use a variable to increment or decrement the value based on the input.
        switch(ch) {
            case KEY_UP:
                i--;
                i = (i<0) ? N_items-1 : i;
            break;
            case KEY_DOWN:
                i++;
                i = (i>N_items-1) ? 0 : i;
            break;
        }
        
        /*Sottolinea la scelta*/
        wattron( w, A_STANDOUT );
        sprintf(item, "%-7s",  list[i]);
        mvwprintw( w, i+1, 2, "%s", item);
        wattroff( w, A_STANDOUT );
    
    }while(ch != 10/* significa enter (non so nemmeno io perché ma funzia) */);

    switch (i)
    {
    case 0:
        /* Single */
        wclear(w);
        wrefresh(w);
        delwin(w);
        newGameSingle();
        break;
    case 1:
        /* PvP */
        break;
    case 2:
        /* PvCPU */
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


int main(){
    /*Inizia il programma*/
    /*Attivo NCURSES con i nostri parametri*/
    initMainScreen();
    if(LINES < 30 || COLS < 60){
        printf("Comprati un monitor piu' grande, poveraccio!");
        endwin();
        return 69;
    }
    printCredits();
    printLogo();
    mainMenu();
    return 0;
}