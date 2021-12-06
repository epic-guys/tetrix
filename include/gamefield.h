#ifndef _GAMEFIELD_H_
#define _GAMEFIELD_H_

#define FIELD_ROWS 15
#define FIELD_COLS 10
#define FIELD_W_ROWS FIELD_ROWS + 1 + 4 /*Finestra del campo->Righe(verticale) = Numero righe + 1 riga(sotto) + 4 blocco di selezione*/
#define FIELD_W_COLS FIELD_COLS * 2 + 2 /*Finestra del campo->Colonne(orizzontale = NUmero colonne * 2(blocco) + 2 bordi(destra-sinistra)*/

typedef struct gamefield_t gamefield_t;

gamefield_t *initializeGameField(int y, int x);
void clearTop(gamefield_t *g);
void refreshGamefield(gamefield_t *g);

#endif