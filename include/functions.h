#ifndef _GPFUNCTIONS_H_
#define _GPFUNCTIONS_H_

char* form(int m_size, char title[]);

void delay(int millisec);

void kill_win(WINDOW* w);

void wprint_with_delay(WINDOW* w,int d,char* c);

void mvwprintwrows(WINDOW* w, int y, int x, char* c);

int swap_color(int c);

#pragma region ASCII ART

int get_ASCII_art_rows(char ascii[]);
int get_ASCII_art_cols(char ascii[]);

#pragma endregion

#endif