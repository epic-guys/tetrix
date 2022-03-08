#include <stdio.h>
#include <ncurses.h>

#include <singleplayer.h>
#include <multiplayer.h>
#include <functions.h>
#include <constants.h>


void initColors();
void initMainScreen();
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
    init_color(8,1000,647, 0);
    init_pair(PAIR_ORANGE,COLOR_ORANGE ,COLOR_BLACK);
   
    /* GIALLO su sfondo NERO */
    init_pair(PAIR_YELLOW,COLOR_YELLOW,COLOR_BLACK);
   
    /*VERDE su sfondo NERO*/
    init_pair(PAIR_GREEN,COLOR_GREEN,COLOR_BLACK);
   
    /*VIOLA su sfondo NERO*/
    init_color(9,218,112,214);
    init_pair(PAIR_MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
   
    /*ROSSO su sfondo NERO*/
    init_pair(PAIR_RED,COLOR_RED,COLOR_BLACK);

    /*sfondo OMBREGGIATURA*/
    /*init_pair(100,COLOR_BLACK,COLOR_MAGENTA);*/

    /* NERO su sfondo CIANO */
    init_pair(PAIR_BG_CYAN,COLOR_BLACK,COLOR_CYAN);
   
    /* NERO su sfondo BLU */
    init_pair(PAIR_BG_BLUE,COLOR_BLACK,COLOR_BLUE);
   
    /* NERO su sfondo ARANCIONE */
    init_pair(PAIR_BG_ORANGE,COLOR_BLACK ,COLOR_ORANGE);
   
    /* NERO su sfondo GIALLO */
    init_pair(PAIR_BG_YELLOW,COLOR_BLACK,COLOR_YELLOW);
   
    /*NERO su sfondo VERDE*/
    init_pair(PAIR_BG_GREEN,COLOR_BLACK,COLOR_GREEN);
   
    /*NERO su sfondo VIOLA*/
    init_pair(PAIR_BG_MAGENTA,COLOR_BLACK,COLOR_MAGENTA);
   
    /*NERO su sfondo ROSSO*/
    init_pair(PAIR_BG_RED,COLOR_BLACK,COLOR_RED);

}

/**
 * @brief stampa il logo del gioco in ASCII art.
 */
void printLogo(){
    WINDOW *logo;
    int i, j, space;
    
    logo = newwin( 10, COLS-2, 0, 0 );

    space = (COLS - getASCIIArtCols(ART_LOGO)) / 2 - 5;
    mvwprintwrows(logo, 0, space, ART_LOGO);
    wrefresh(logo);
    /* Nasconde il cursore di sistema*/
    curs_set(0);
}

/**
 * @brief stampa la finestra in basso con i nostri crediti.
 */
void printCredits(){
    WINDOW *credits;
    int x,y;
    credits = newwin( 3, COLS-2, LINES-3 , 1 );
    box(credits, 0, 0 );
    wmove(credits,getcurx(credits)+1,getcury(credits)+2);
    wprintw(credits,"PROGETTO DI GRUPPO - 2022 - DAVIDE ANTONIO AMODIO - LUCA BOFFO - ALVISE FAVERO");
    wrefresh(credits);
}


/**
 * @brief stampa e gestisce il menú principale.
 */
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
        
        sprintf(item, "%-7s",  list[i]); 
        mvwprintw( w, i+1, 2, "%s", item ); 
        
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
        killWin(w);
        newGameSingle();
        mainMenu();
        break;
    case 1:
        /* PvP */
        killWin(w);
        newGameMulti();
        mainMenu();
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
    if(LINES < 30 || COLS < 125){
        printf("Comprati un monitor piu' grande, poveraccio!");
        endwin();
        return 69;
    }
    printCredits();
    printLogo();
    mainMenu();
    return 0;
}