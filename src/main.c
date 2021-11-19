#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ncurses.h>


#pragma region CONSTANT DATA

#define FIELD_ROWS 15
#define FIELD_COLS 10
#define TETRIMINOS_FOR_TYPE 20

#define FIELD_W_ROWS FIELD_ROWS + 5
#define FIELD_W_COLS FIELD_COLS * 2 + 2

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
    for (i = 0; i < sizeof ASCII_logo; i++)
    {
        printw("%c", ASCII_logo[i]);
        if (ASCII_logo[i] == '\n')
        {
            for (j = 0; j < space; ++j)
            {
                printw(" ");
            }
        }
    }
}

/**
 * Riempie le righe mancanti andando a capo simulando il refresh del terminale 
 */
void printEmpty(int remove, int sys_h)
{
    int row;
    for (row = 0; row < sys_h - getSplashLogoHeight() - 1 - remove; row++)
    {
        printf("\n");
    }
}

#pragma endregion

#pragma region STRUCTURES AND ENUMS
/**
 * @brief Codifica di ogni tetramino.
 * 
 * T_I:
 * ████████
 * T_J:
 *   ██
 *   ██
 * ████
 * T_L:
 * ██
 * ██
 * ████
 * T_S:
 *   ████
 * ████
 * T_O:
 * ████
 * ████
 * T_Z:
 * ████
 *   ████
 * T_T:
 * ██████
 *   ██
 */
enum TetriminoType
{
    T_I,
    T_J,
    T_L,
    T_S,
    T_O,
    T_Z,
    T_T
} const allTypes[7] = { T_I, T_J, T_L, T_O, T_S, T_T, T_Z };

/**
 * @brief Non è un typo, in inglese
 * si scrive così.
 * 
 * L'idea è di salvare in maniera
 * generica ogni tetramino in una matrice
 * 4x4 e salvarci affianco la larghezza
 * e l'altezza, in questo modo diventa
 * più facile gestire la caduta dall'alto,
 * i bordi del campo e la rotazione.
 */
struct Tetrimino
{
    int* values;
    int cols;
    int rows;
    enum TetriminoType type;
};

/**
 * Per gestire la rotazione si potrebbe
 * usare un int, ma con l'enum si limitano
 * le rotazioni possibili a questi 4 angoli
 * precisi
 * La D iniziale sta per "degrees"
*/
enum Rotation
{
    D_0,
    D_90,
    D_180,
    D_270
};

/**
 * Struttura che associa ad ogni tetramino
 * la sua quantità rimanente
 */
struct TetriminoSet
{
    enum TetriminoType tetrimino;
    size_t remaining;
};

typedef struct Player
{
    int field[FIELD_ROWS][FIELD_COLS];
    WINDOW *win;
    int cursor_pos;
} player_t;

#pragma endregion

#pragma region GAME VARIABLES

int field[FIELD_ROWS][FIELD_COLS];
struct TetriminoSet* sets;
player_t *p1, p2;

#pragma endregion

#pragma region GAME FUNCTIONS

/**
 * @brief Istanzia uno struct
 * giocatore e inizializza il campo
 * con valori 0 e crea una finestra nella
 * posizione indicata.
 * 
 * @param[in] y_pos La posizione Y iniziale in cui posizionare la finestra.
 * @param[in] x_pos La posizione X iniziale in cui posizionare la finestra.
 * @return Lo struct giocatore istanziato. 
 */
player_t *initializePlayer(int y_pos, int x_pos)
{
    player_t *player = (player_t*) malloc(sizeof(player_t));
    WINDOW *w;
    size_t i, j;
    for (i = 0; i < FIELD_ROWS; i++)
        for (j = 0; j < FIELD_COLS; j++)
            player->field[i][j] = 0;
    w = newwin(FIELD_W_ROWS, FIELD_W_COLS, y_pos, x_pos);
    attron(A_BOLD);
    wborder(w, '|', '|', ' ', '=', ' ', ' ', '\\', '/');
    attroff(A_BOLD);
    wrefresh(w);
    player->win = w;
    player->cursor_pos = 4;
    return player;
}

/**
 * @brief Da chiamare per visualizzare
 * le modifiche al campo del giocatore.
 * 
 * @param[in, out] p Il giocatore di cui bisogna aggiornare lo schermo.
 */
void refreshPlayer(player_t *p)
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        for (j = 0; j < FIELD_COLS; ++j)
        {
            wmove(p->win, FIELD_W_ROWS - FIELD_ROWS + i - 1, j * 2 + 1);
            if (p->field[i][j])
                wprintw(p->win, "[]");
            else
                wprintw(p->win, "  ");
        }
    }
    wrefresh(p->win);
}

/**
 * @brief Funzione WIP che aggiorna la posizione del
 * tetramino sulla zona di posizionamento.
 * Più avanti dovrà ricevere come parametro un tetramino
 * e dovrà rispettare la sua dimensione nel campo da gioco. 
 * 
 * @param[in, out] p Il giocatore in cui aggiornare la posizione.
 * TODO param tetrimino
 */
void refreshSelector(player_t *p)
{
    int i;
    for (i = 0; i < 4; ++i)
    {
        wmove(p->win, i, 0);
        wclrtoeol(p->win);
    }
    wmove(p->win, 3, p->cursor_pos * 2 + 1);
    wprintw(p->win, "[]");
    wrefresh(p1->win);
}

/**
 * @brief Genera lo struct del tetramino associato
 * alla sua codifica
 * 
 * @param[in] type La codifica del tetramino
 * @return Lo struct del Tetramino 
 */
struct Tetrimino getTetrimino(enum TetriminoType type)
{
    struct Tetrimino t;
    switch (type)
    {
        case T_I: {
            int values[1][4] = {
                    1, 1, 1, 1
            };
            t.values = &values[0][0];
            t.cols = 4;
            t.rows = 1;
            break;
        }
        case T_J: {
            int values[2][3] = {
                    1, 0, 0,
                    1, 1, 1,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_L: {
            size_t i,j;
            int values[2][3] = {
                    0, 0, 1,
                    1, 1, 1
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_S: {
            int values[2][3] = {
                    0, 1, 1,
                    1, 1, 0,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_Z: {
            int values[2][3] = {
                    1, 1, 0,
                    0, 1, 1,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        case T_O: {
            int values[2][2] = {
                    1, 1,
                    1, 1,
            };

            t.values = &values[0][0];
            t.cols = 2;
            t.rows = 2;
            break;
        }
        case T_T: {
            int values[2][3] = {
                    0, 1, 0,
                    1, 1, 1,
            };

            t.values = &values[0][0];
            t.cols = 3;
            t.rows = 2;
            break;
        }
        default:
            printf("Non so come hai fatto ma hai passato un valore non presente nell'enum");
    }
    return t;
}

/**
 * @brief Inizializza tutti gli insiemi dei tetramini
 * @return puntatore alla struttura
 */
void initializeSets()
{
    int i;
    sets = (struct TetriminoSet*)malloc(sizeof(struct TetriminoSet) * 7);
    for (i = 0; i < 7; ++i)
    {
        struct TetriminoSet s = { allTypes[i], TETRIMINOS_FOR_TYPE };
        sets[i] = s;
    }
}

#pragma endregion

#pragma region PRINTERS

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

    printEmpty(getSplashLogoHeight(), getWinHeight());
}

void printField()
{
    int i, j;
    for (i = 0; i < FIELD_ROWS; ++i)
    {
        printf("║");
        for (j = 0; j < FIELD_COLS; ++j)
        {
            if (field[i][j])
                printf("██");
            else
                printf("  ");
        }
        printf("║\n");
    }
    printf("╚");
    for (i = 0; i < FIELD_COLS; i++)
    {
        printf("══");
    }
    printf("╝\n");
}


/**
 * @brief Stampa la scena di gioco,
 * va chiamata ogni volta che si
 * verifica un cambiamento
 * @deprecated Stiamo passando a ncurses
 */
void printScene()
{
    printEmpty(FIELD_ROWS, getWinHeight() + 5);
    printField();
    printf("\n");


    /*
    TODO per ora si limita a stampare
    il campo di gioco, più avanti dovrà
    centrare il campo, mostrare tutte
    le celle occupate, pezzi selezionati
    e pezzi disponibili
    */
}

#pragma endregion

/**
 * Inizia la partita
 */
void newGameSingle()
{
    refresh();
    /* p1 = initializePlayer((LINES - FIELD_ROWS) / 2, (COLS - FIELD_COLS) / 2); */
    p1 = initializePlayer(0, 0);
    initializeSets();

    /* TEST */
    p1->field[FIELD_ROWS - 1][0] = 1;
    p1->field[FIELD_ROWS - 1][1] = 1;
    p1->field[FIELD_ROWS - 1][2] = 1;
    p1->field[FIELD_ROWS - 2][1] = 1;
    refreshPlayer(p1);

    while (1)
    {
        int i, selecting = 1;
        refreshSelector(p1);
        while (selecting)
        {
            int ch = getch();
            switch (ch)
            {
                case KEY_RIGHT:
                    if (p1->cursor_pos < 9)
                        ++p1->cursor_pos;
                    break;
                case KEY_LEFT:
                    if (p1->cursor_pos > 0)
                        --p1->cursor_pos;
                    break;
                case '\n':
                    selecting = 0;
                    break;
            }
            refreshSelector(p1);
        }
        
        for (i = 0; i < FIELD_ROWS; i++)
        {
            if (
                p1->field[i][p1->cursor_pos] == 0 && (i == FIELD_ROWS - 1 || p1->field[i + 1][p1->cursor_pos] == 1))
            {
                p1->field[i][p1->cursor_pos] = 1;
                break;
            }
        }
        refreshPlayer(p1);
    }
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
    /* Attiva la modalità curses */
    initscr();
    /* Disattiva buffer per l'input
       (non aspetta EOF o l'invio prima di
       restituire l'input) */
    cbreak();
    /* Attiva tasti speciali,
       ad esempio le freccette, F1, F2... */
    keypad(stdscr, TRUE);
    /* Non stampa su schermo l'input */
	noecho();
    newGameSingle();
    /*
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
    */
    return 0;
}