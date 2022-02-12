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

#pragma endregion

#pragma region FIELD

#define FIELD_ROWS 15
#define FIELD_COLS 10
/*Finestra del campo->Righe(verticale) = Numero righe + 1 riga(sotto) + 4 blocco di selezione*/
#define FIELD_W_ROWS FIELD_ROWS + 1 + 4
/*Finestra del campo->Colonne(orizzontale = NUmero colonne * 2(blocco) + 2 bordi(destra-sinistra)*/
#define FIELD_W_COLS FIELD_COLS * 2 + 2

#pragma endregion

#pragma region POOL

#define POOL_ROWS 23
#define POOL_COLS 50

#pragma endregion

#pragma region POINTBOARD

#define POINTBOARD_ROWS_1P 5
#define POINTBOARD_ROWS_2P 7
#define POINTBOARD_COLS 28

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

#pragma endregion

#endif
