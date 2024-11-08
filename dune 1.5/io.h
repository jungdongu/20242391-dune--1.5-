/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_


#include "common.h"

KEY get_key(void);
void printc(POSITION pos, char ch, int color , int bag);
void gotoxy(POSITION pos);
void set_color(int color, int bag);
void printc2(POSITION pos, char ch[100], int color, int bag);
#endif

