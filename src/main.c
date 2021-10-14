#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>

#pragma region CONSTANT DATA

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RESETCOLORS "\e[0m"

#define FIELD_H 15
#define FIELD_W 10

const int menuHeight = 5;
/* Il numero di pezzi iniziali per ciascun tipo */
const int STARTPIECES = 20;
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

#pragma endregion

#pragma region GENERAL PURPOSE FUNCTIONS

/**
 * Funzione per svuotare il buffer di input, 
 * chiamare dopo gli input per sanificarli 
 * ed evitare che in caso di input errato
 * ES: array di char al posto di un int
 * che bugga tutto e manda il programma in loop 
 */
void empty_stdin(void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

/**
 * Calcola quante righe occuperà
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
 * calcola quante colonne occuperà
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

/**
 * Ottiene la larghezza del
 * terminale dove stiamo
 * eseguendo il programma
 */
int getWinWidth()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
}

/**
 * Ottiene l'altezza del
 * terminale dove stiamo
 * eseguendo il programma
 */
int getWinHeight()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_row;
}

/**
 * Stampa il logo centrandolo
 * e controllando che il terminale
 * non sia troppo piccolo
 */
void printSplashLogo()
{
    int i, j, space, input;
    int w = getWinWidth();
    int h = getWinHeight();
    printf("\n");
    while (w < getSplashLogoWidth() || h < getSplashLogoHeight())
    {
        printf("L'altezza o la larghezza del tuo terminale non é sufficiente, \n"
               "per favore, correggi allargando il terminale \n"
               "premi 0 per continuare");
        scanf("%d", &input);
        if (input != 0)
        {
            printf("Selezione errata, riprova\n");
            empty_stdin();
        }
        w = getWinWidth();
        h = getWinHeight();
    }
    space = (w - getSplashLogoWidth()) / 2 - 10;
    for (j = 0; j < space; ++j)
    {
        printf(" ");
    }
    printf(CYN);
    for (i = 0; i < sizeof ASCII_logo; i++)
    {
        printf("%c", ASCII_logo[i]);
        if (ASCII_logo[i] == '\n')
        {
            for (j = 0; j < space; ++j)
            {
                printf(" ");
            }
        }
    }
    printf(RESETCOLORS);
}

#pragma endregion

int field[FIELD_W][FIELD_H];

#pragma region GAME FUNCTIONS

void initializeField()
{
    int i, j;
    for (i = 0; i < FIELD_W; i++)
        for (j = 0; j < FIELD_H; j++)
            field[i][j] = 0;
}

#pragma endregion

/**
 * Per il momento riempe
 * il resto dello schermo
 * con *NULLA* lasciando
 * spazio per il menu
 */
void welcomeScreen()
{
    int row;
    int sys_w;
    int sys_h;
    printSplashLogo();
    sys_h = getWinHeight();
    sys_w = getWinWidth();
    for (row = 0; row < sys_h - getSplashLogoHeight() - 1 - menuHeight; row++)
    {
        printf("\n");
    }
    return;
}

/**
 * Stampa la scena di gioco,
 * va chiamata ogni volta che si
 * verifica un cambiamento
 */
void printScene()
{
    int i, j;
    for (i = 0; i < FIELD_H; ++i)
    {
        printf("║");
        for (j = 0; j < FIELD_W; ++j)
        {
            if (field[i][j])
                printf("██");
            else
                printf("  ");
        }
        printf("║\n");
    }
    printf("╚");
    for (i = 0; i < FIELD_W; i++)
    {
        printf("══");
    }
    printf("╝\n");

    /*
    TODO per ora si limita a stampare
    il campo di gioco, più avanti dovrà
    centrare il campo, mostrare tutte
    le celle occupate, pezzi selezionati
    e pezzi disponibili
    */
}

/**
 * Inizia la partita
 */
void newGame()
{
    initializeField();

    /* TEST */
    field[FIELD_H - 1][0] = 1;
    field[FIELD_H - 1][1] = 1;
    field[FIELD_H - 1][2] = 1;
    field[FIELD_H - 2][1] = 1;
    printScene();
    /*
    TODO inizializzare valori di gioco,
    impostando tutti i pezzi disponibili
    a STARTPIECES
    */
}

int printMenu(void)
{
    int s = 10;

    printf("\n");
    while (s > 3 || s < 1)
    {
        printf("------------------------------------------------------\n");
        printf("| premi 1 per iniziare una nuova partita             |\n");
        printf("| premi 2 per iniziare una nuova partita multiplayer |\n");
        printf("| premi 3 per uscire                                 |\n");
        printf("------------------------------------------------------\n");
        scanf("%d", &s);
        if (s > 3 || s < 1)
        {
            printf("Selezione errata, riprova\n");
            empty_stdin();
        }
    }
    return s;
}

int main(void)
{
    int v;
    welcomeScreen();
    while (1)
    {
        v = printMenu();
        switch (v)
        {
        case 1:
            while (1)
            {
                newGame();
                return 0;
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