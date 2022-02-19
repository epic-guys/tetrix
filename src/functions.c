#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <functions.h>
#include <tetrimino.h>
#include <time.h>

/**
 * @brief Funzione per creare un form al centro dello schermo
 * la risposta viene caricata in un array di char (allocata dal caller)
 * 
 * @param[in] memory puntatore della malloc dove mettere la risposta.
 * @param[in] m_size dimensione massima della risposta che si vuole ricevere.
 * @param[in] title titolo della finestra.
 */
void form(char *memory, int m_size, char title[]){
    int i, ch, j, flag=0;
    WINDOW *w;

    m_size--;
    
    w = newwin(3, (strlen(title)>m_size ? strlen(title) : m_size) +6 ,(LINES/2)-1,(COLS/2)-(((strlen(title)>m_size ? strlen(title) : m_size)/2)+3));
    box(w,0,0);
    
    mvwprintw(w,0,1,"%s",title);
    
    /*stampa gli underscore, mette il cursore, pulisce la memoria, aggiunge il null terminator*/
    for(i=0;i<m_size;++i){
        if(i==0){
            wattron( w, A_STANDOUT );
        }
        else{
            wattroff( w, A_STANDOUT );
        }
        mvwprintw(w,1,2+i,"_");
    }

    wrefresh(w);

    i=0;
    while(ch != 10){
        ch = wgetch(w);
        
        if(ch == 127){
            if(i>0){
                memory[--i] = '\0';
            }
        }
        else{
            if(i<m_size){
                memory[i++] = ch;
            }
        }

        for(j=0;j<m_size;++j){
            if(memory[j]=='\0'){
                if(flag==0){
                    wattron( w, A_STANDOUT );
                    flag=1;
                }
                else{
                    wattroff( w, A_STANDOUT );
                }
                mvwprintw(w,1,2+j,"_");
            }
                
            else
                mvwprintw(w,1,2+j,"%c",memory[j]);
            
        }
        flag=0;
        wrefresh(w);
    }

    wclear(w);
    wrefresh(w);
    delwin(w);
    endwin();
    
    memory[++i] = '\0';
    memory=realloc(memory,i);

    return;
}

/**
 * @brief una funzione delay. [ATTENZIONE, É BLOCCANTE]
 * 
 * @param[in] millisec tempo in millisecondi.
 */
void delay(int millisec){
    if (millisec==0) return;
    millisec = millisec * CLOCKS_PER_SEC / 1000;
    clock_t start_time = clock();
    while (clock() < start_time + millisec);
}

/**
 * @brief elimina le finestre di ncurses.
 * 
 * @param[in] w la finestra da eliminare.
 */
void killWin(WINDOW* w){
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
void wprintWithDelay(WINDOW* w,int d,char* c){
    int i=0;
    char ch;
    while(c[i] != '\0'){
        if(c[i]!='\n')
            wprintw(w,"%c",c[i]);
        else
            wmove(w,getcury(w)+1,2);
        i++;
        wrefresh(w);
        delay(d);
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
 * @param[out] c colore sotto forma numerica.
 */
int swapColor(int c){
    if(c<100)
        return c+100;
    return c-100;
}