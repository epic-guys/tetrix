#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ncurses.h>

void initColors();

/**
 * @brief inizializza la finestra main di ncurses con i nostri parametri (é una funzione stub)
 */
void initMainScreen(){
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    initColors();
}
/**
 * @brief inizializziamo tutti i colori (é una funzione stub)
 */
void initColors(){
    start_color();
    /* CIANO su sfondo NERO */
    init_pair(1,COLOR_CYAN,COLOR_BLACK);
   
   /* BLU su sfondo NERO */
    init_pair(2,COLOR_BLUE,COLOR_BLACK);
   
   /* ARANCIONE su sfondo NERO */
    init_color(8,255,240,0);
    init_pair(3,8,COLOR_BLACK);
   
   /* GIALLO su sfondo NERO */
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
   

   /*VERDE su sfondo NERO*/
    init_pair(5,COLOR_GREEN,COLOR_BLACK);
   
   /*VIOLA su sfondo NERO*/
    init_color(9,218,112,214);
    init_pair(6,9,COLOR_BLACK);
   
   /*ROSSO su sfondo NERO*/
    init_pair(7,COLOR_RED,COLOR_BLACK);
}

void mainMenu(){
    WINDOW *w;
    /*Quanti elementi ci sono nel menú*/
    int N_items = 4;
    char list[4][15] = { "Single Player", "PvP", "PvCPU", "Quit"};
    char item[15];
    int ch, i = 0, width = 7;
    /* Crea una nuova finestra */
    w = newwin( 10, COLS-2, 1, 1 );
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
    mainMenu();
}