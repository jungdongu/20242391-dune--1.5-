/*
* raw(?) I/O
*/
#include "io.h"
#include "common.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color,int bag) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | (bag <<4));
}

void printc(POSITION pos, char ch, int color, int bag) {
	if (color >= 0) {
		set_color(color,bag);
	}
	gotoxy(pos);
	printf("%c", ch);
}

void printc2(POSITION pos, char ch[100], int color, int bag) {
	if (color >= 0) {
		set_color(color, bag);
	}
	gotoxy(pos);
	printf("%s", ch);
}

void printc3(POSITION pos, int ch, int color, int bag) {
	if (color >= 0) {
		set_color(color, bag);
	}
	gotoxy(pos);

	printf("%d", ch);
}






KEY get_key(void) {
	if (!_kbhit()) {  // 입력된 키가 있는지 확인
		return k_none;
	}

	int byte = _getch();    // 입력된 키를 전달 받기
	switch (byte) {
	case 'q': return k_quit; // 'q'를 누르면 종료
	case 'Q': return k_quit;
	case ' ': return k_tp;
	case 'b': return k_build;
	case 'B': return k_build;
	case 'h': return k_habester;
	case 'H': return k_habester;
	case 'P': return k_Plate;
	case 'p': return k_Plate;
	case 'D': return k_Dormitory;
	case 'd': return k_Dormitory;
	case 'G': return k_Garge;
	case 'g': return k_Garge;
	case 'a': return k_Barracks;
	case 'A': return k_Barracks;
	case 'R': return k_Shelter;
	case 'r': return k_Shelter;
	case 'm': return k_move;
	case 'M': return k_move;
	case 'l': return k_move;
	case 'L': return k_move;
	case 27: return k_re;
	case 224:
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte) {
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		default: return k_undef;
		}
	default: return k_undef;
	}
}
