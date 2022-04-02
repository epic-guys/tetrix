#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include <tetrimino.h>
#include <time.h>

/**
 * @brief Funzione per creare un form al centro dello schermo
 * la risposta viene caricata in un array di char (allocata dal caller)
 * 
 * @param[in] m_size dimensione massima della risposta che si vuole ricevere.
 * @param[in] title titolo della finestra.
 * @return L'array contenente la stringa, di dimensione m_size + 1.
 */
char* form(int m_size, char title[]){
    int i, ch, j, flag=0;
    WINDOW *w;
    char* memory = calloc(m_size + 1, sizeof(char));

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
    /*
    Bisogna fare un loop infinito perché l'input viene preso all'interno della funzione.
    Sarà poi la funzione a doverlo rompere appena incontra il carattere '\n'.
    Utilizzando il metodo vecchio, viene aggiunto il carattere '\n' alla stringa.
    */
    while(1){
        ch = wgetch(w);
        /*
        Avrei voluto usare uno switch ma sarebbe stato un problema fare break,
        dato che è necessario per lo switch.
        */
        if (ch == '\n') break;
        else if(ch == 127){
            if(i>0){
                memory[--i] = '\0';
            }
        }
        else{
            if(i<m_size){
                memory[i++] = ch;
                //memory[i] = '\0';
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

    killWin(w);
    
    memory[++i] = '\0';

    return memory;
}

/**
 * @brief Conta il numero di righe di una
 * ASCII art rettangolare importata in C attraverso xxd.
 * 
 * @param ascii La stringa contenente la ASCII art.
 * @return Il numero di righe.
 */
int getASCIIArtRows(char ascii[])
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
 * ASCII art rettangolare importata in C attraverso xxd.
 * @param ascii La stringa contenente la ASCII art.
 * @return Il numero di colonne
 */
int getASCIIArtCols(char ascii[])
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

void mvwprintwrows(WINDOW* w, int y, int x, char* c)
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