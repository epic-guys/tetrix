#ifndef _GPFUNCTIONS_H_
#define _GPFUNCTIONS_H_

char* form(int m_size, char title[]);

void delay(int millisec);

void killWin(WINDOW* w);

void wprintWithDelay(WINDOW* w,int d,char* c);

void mvwprintwrows(WINDOW* w, int y, int x, char* c);

int swapColor(int c);

#pragma region ASCII ART

int getASCIIArtRows(char ascii[]);
int getASCIIArtCols(char ascii[]);

#pragma endregion

#endif