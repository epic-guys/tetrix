#ifndef _GPFUNCTIONS_H_
#define _GPFUNCTIONS_H_

void form(char *memory, int m_size, char title[]);

void delay(int millisec);

void killWin(WINDOW* w);

void wprintWithDelay(WINDOW* w,int d,char* c);

int swapColor(int c);

#endif