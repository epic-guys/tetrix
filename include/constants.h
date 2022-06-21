/**
 * @file constants.h
 * @brief Contiene tutte le macro di costanti.
 * Mi è sembrato necessario crearla perché i dati costanti
 * erano troppo sparsi nel progetto. Le costanti venivano salvate
 * all'interno degli header però venivano usate anche nei file
 * che non le includevano. Il progetto compilava solo perché venivano
 * incluse prima di compilare il file che le usa.
 */

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#pragma region COLORS

#define COLOR_ORANGE 8
#define PAIR_CYAN 1
#define PAIR_BLUE 2
#define PAIR_ORANGE 3
#define PAIR_YELLOW 4
#define PAIR_GREEN 5
#define PAIR_MAGENTA 6
#define PAIR_RED 7

#define PAIR_BG_CYAN 101
#define PAIR_BG_BLUE 102
#define PAIR_BG_ORANGE 103
#define PAIR_BG_YELLOW 104
#define PAIR_BG_GREEN 105
#define PAIR_BG_MAGENTA 106
#define PAIR_BG_RED 107

#pragma endregion

#pragma region FIELD

#define FIELD_ROWS 15
#define FIELD_COLS 10
/*Finestra del campo->Righe(verticale) = Numero righe + 1 riga(sotto) + 4 blocco di selezione*/
#define FIELD_W_ROWS FIELD_ROWS + 1 + 4
/*Finestra del campo->Colonne(orizzontale = NUmero colonne * 2(blocco) + 2 bordi(destra-sinistra)*/
#define FIELD_W_COLS FIELD_COLS * 2 + 2
#define TOP_ROWS 4

#pragma endregion

#pragma region POOL

#define POOL_ROWS 23
#define POOL_COLS 50

#pragma endregion

#pragma region POINTBOARD

#define POINTBOARD_ROWS_1P 5
#define POINTBOARD_ROWS_2P 7
#define POINTBOARD_COLS 28

#define NICKNAME_LEN 16

#pragma endregion

#pragma region TETRIMINO

#define N_tetrimini 7
#define TETRIMINOS_PER_TYPE 20

#pragma endregion

#pragma region GAME_SETTINGS

#define POINTS_ONE_ROW_DELETED 1
#define POINTS_TWO_ROW_DELETED 3
#define POINTS_THREE_ROW_DELETED 6
#define POINTS_FOUR_ROW_DELETED 12
#define TXT_DELAY 3

#pragma endregion

#pragma region ART

extern const unsigned char ART_LOGO[];

extern const char ART_GAME_OVER[];

#pragma endregion

#pragma region GAME_TXTS
extern const char *SINGLE_WELCOME_TXT[];
extern const char *SINGLE_END_TXT[];
extern const char *PVP_WELCOME_TXT[];
extern const char *PVP_END_TXT[];
extern const char *PVE_WELCOME_TXT[];

#pragma endregion

#pragma region CPU

/*
Con "error" si intende la dimensione dell'array in cui la CPU
selezionerà le migliori mosse. Più è grande, più è probabile che la CPU
non faccia la mossa migliore.
*/

#define CPU_ERROR_EASY 10
#define CPU_ERROR_MEDIUM 5
#define CPU_ERROR_HARD 3
#define CPU_ERROR_IMPOSSIBLE 1

#pragma endregion

#endif
