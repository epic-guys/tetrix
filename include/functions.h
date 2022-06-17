#ifndef _GPFUNCTIONS_H_
#define _GPFUNCTIONS_H_

char* form(int m_size, char title[]);

void delay(int millisec);

void kill_win(WINDOW* w);

void wprint_with_delay(WINDOW* w,int d, const char* c);

void mvwprintwrows(WINDOW* w, int y, int x, char* c);

int swap_color(int c);

int next_circular(int i, int size);

int prev_circular(int i, int size);

int super_free(void **ptr);

#pragma region ASCII ART

int get_ASCII_art_rows(const unsigned char ascii[]);

int get_ASCII_art_cols(const unsigned char ascii[]);

#pragma endregion

#endif