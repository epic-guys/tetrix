#include <ncurses.h>
#include <string.h>

#include <functions.h>
#include <tetrimino.h>
#include <time.h>

/**
 * @brief Funzione generale per creare un form al centro dello schermo
 * la risposta viene caricata in un array di char (allocata dal caller)
 * @param[in] memory puntatore della malloc dove mettere la risposta
 * @param[in] m_size dimensione massima della risposta che si vuole ricevere
 * @param[in] title titolo della finestra
 */
void form(char *memory, int m_size, char title[]){
    int i, ch, j, flag=0;
    WINDOW *w;

    memory[m_size] = '\n';

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
        memory[i] = '\0';
    }

    wrefresh(w);

    i=0;
    while(ch != 10){
        ch = wgetch(w);
        
        if(ch == 127){
            if(i>0){
                i=i-1;
                memory[i] = '\0';
            }
        }
        else{
            if(i<m_size){
                memory[i] = ch;
                ++i;
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
    
    return;
}

/**
 * @brief una funzione delay [ATTENZIONE, É BLOCCANTE]
 * @param[in] millisec tempo in millisecondi
 */
void delay(int millisec){
    millisec*=100;
    clock_t start_time = clock();
    while (clock() < start_time + millisec)
        ;
}