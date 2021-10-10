#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>

/*BEGIN OF CONSTANT DATA*/ 
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
  0x2a, 0x23
};

/*END OF CONSTANT DATA*/

/*BEGIN OF GENERAL PURPOSE FUNCTIONS*/
void empty_stdin (void)
{
    /** funzione per svuotare il buffer di input, 
     *  chiamare dopo gli input per sanificarli 
     *  ed evitare che in caso di input errato
     *  ES: array di char al posto di un int
     *  che bugga tutto e manda il programma in loop 
     */
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

int getSplashLogoHeight(){
    /** calcola quante righe occuperá 
     *  il logo per essere stampato
     *  utilizzato per cambiare 
     *  facilmente il logo senza hardcodare
     *  l'altezza del file nel codice
     */
    int i,c=1;
    for(i=0;i<sizeof ASCII_logo;++i){
        if(ASCII_logo[i] ==  '\n'){
            c++;
        }
    }
    return c;
}

int getSplashLogoWidth(){
    /** calcola quante colonne occuperá 
     *  il logo per essere stampato
     *  utilizzato per cambiare 
     *  facilmente il logo senza hardcodare
     *  la larghezza del file nel codice [NON HO ANCORA FATTO]
     */
    int i,c=1;
    for(i=0;i<sizeof ASCII_logo;++i){
        if(ASCII_logo[i] ==  '\n'){
            c++;
        }
    }
    return c;
}

int getWinWidth(){
    /** ottiene la larghezza del 
     *  terminale dove stiamo 
     *  eseguendo il programma
     */
    struct winsize w;  
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
}

int getWinHeight(){
    /** ottiene l'altezza del 
     *  terminale dove stiamo 
     *  eseguendo il programma
     */
    struct winsize w;  
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_row;
}

void printSplashLogo(){
    /* Stampa il logo centrandolo */
    int i;
    printf("\n");
    for(i=0;i< sizeof ASCII_logo;i++){
        printf("%c",ASCII_logo[i]);
    }
}
/*END OF GENERAL PURPOSE FUNCTIONS*/


void welcomeScreen(int w,int h){
    /* per il momento riempe il resto dello schermo con *NULLA* */
    int row;
    printSplashLogo();
    for(row=0;row<h-getSplashLogoHeight()-1;row++){
        printf("\n");    
    }
    return;
}

int printMenu(void){
    int s = 10;
    
    printf("\n");
    while(s > 3 || s < 1){
        printf("------------------------------------------------------\n");
        printf("| premi 1 per iniziare una nuova partita             |\n");
        printf("| premi 2 per iniziare una nuova partita multiplayer |\n");
        printf("| premi 3 per uscire                                 |\n");
        printf("------------------------------------------------------\n");
        scanf("%d",&s);
        if(s > 3 || s < 1){
            printf("Selezione errata, riprova\n");
            empty_stdin();
        }
    }
    return s;
}

int main (void){
    int v;
    welcomeScreen(getWinWidth(),getWinHeight());
    while(1){
        v = 1; /*printMenu();*/
        switch (v)
        {
        case 1:
            /* code */
            while (1)
            {
                
            }
            
            break;
        case 2:
            printf("the developer is on strike so I guess there is no multiplayer game :) \n");
            break;
        case 3:
            printf("Ok, ciao... \n");
            return 0;
            break;
        default:
            printf("something bad has happened, Tetris has flatlined \n");
            return 1;
            break;
        }
    }
    return 0;
}