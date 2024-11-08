/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char d_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char r_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char c_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char bui_buf[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char b_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char b2_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char cur_loc[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char sys_message1[100] = { 0 };
char sys_copy1[100] = { 6 };
char sys_copy2[100] = { 2 };
char sys_copy3[100] = { 3 };
char sys_copy4[100] = { 4 };
int c1 = 1;
int c2 = 2;
int c3 = 3;
int c4 = 4;
int c5 = 5;
int p_spice = 0;
int b_col[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int hei_s = 23;
int hei_s1 = 22;
int hei_s2 = 21;
int hei_s3 = 20;
int wid_s = 5;
int hei_i = 1;
int wid_i = 78;
int hei_c = 21;
int wid_c = 78;
int hei_h = 14;
int wid_h = 2;



void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void d_build(void);
void d_build2(void);

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_system_message(map);
	display_object_info(map);
	display_commands(map);
	d_build();
	d_build2();
	//
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE,0);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
	p_spice = resource.spice;
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (k == 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = {i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT-10; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (d_buf[i][j] != backbuf[i][j]) {
				if (i == 0) {
					POSITION pos = { i + 18,j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i + 18,j };
					printc(padd(map_pos, pos), backbuf[i+10][j], COLOR_DEFAULT,0);
				}
			}
			d_buf[i][j] = backbuf[i][j];
		}
	}
}
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH-5; j++) {
			if (r_buf[i][j] != backbuf[i][j]) {
				if (j == 54) {
					POSITION pos = { i ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j+5], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
			}
			r_buf[i][j] = backbuf[i][j];
		}
	}
}

void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT-10; i++) {
		for (int j = 0; j < MAP_WIDTH-5; j++) {
			if (c_buf[i][j] != backbuf[i][j]) {
				if (i == 0) {
					POSITION pos = { i+18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
				else if (j == 54) {
					POSITION pos = { i + 18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j + 5], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i+18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i + 10][j], COLOR_DEFAULT,0);
				}
			}
			c_buf[i][j] = backbuf[i][j];
		}
	}
}

void d_build() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if ((1 <= i && i <= 2) && (57 <=j && j <=58)) {
				bui_buf[0][i][j] = 'B';
			}
			else if ((i >= 15 && i <= 16) && (j >= 1 && j <= 2)) {
				bui_buf[0][i][j] = 'B';
			}
			else if ((i == 3 && j == 58) || (i == 14 && j == 1)) {
				bui_buf[0][i][j] = 'H';
			}
			else if (((i >= 1 && i <= 2) && (j >= 55 && j <= 56)) || ((i >= 15 && i <= 16) && (j >= 3  && j <= 4))) {
				bui_buf[0][i][j] = 'P';
			}
			else if ((i == 5 && j == 58) || (i == 12 && j == 1)) {
				bui_buf[0][i][j] = '5';
			}
			else if ((i == 4 && j == 12) || (i == 14 && j == 39)) {
				bui_buf[0][i][j] = 'W';
			}
			else if ((i == 5 && j == 48) || (i == 14 && j == 22) || (i == 15 && j == 54)) {
				bui_buf[0][i][j] = 'R';
			}
			else if (((i >= 4 && i <= 5) && (j >= 33 && j <= 34)) || ((i >= 14 && i <= 15) && (j >= 35 && j <= 36))) {
				bui_buf[0][i][j] = 'R';
			}
			b2_buf[i][j] = bui_buf[0][i][j];
		}
	}
}
	

void d_build2(void) {
	d_build(bui_buf);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (b_buf[i][j] != bui_buf[0][i][j]) {
				if (bui_buf[0][i][j] == 'B') {
					if (i == 1 || i == 2) {
						POSITION pos = { i,j };
						b_col[i][j] = 12;
						printc(padd(map_pos, pos), bui_buf[0][i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos2 = { i,j };
						b_col[i][j] = 9;
						printc(padd(map_pos, pos2), bui_buf[0][i][j], 15, b_col[i][j]);
					}
				}
				else if (bui_buf[0][i][j] == 'H') {
					if (i == 3) {
						POSITION pos3 = { i,j };
						b_col[i][j] = 4;
						printc(padd(map_pos, pos3), bui_buf[0][i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos4 = { i,j };
						b_col[i][j] = 1;
						printc(padd(map_pos, pos4), bui_buf[0][i][j], 15, b_col[i][j]);
					}
				}
				else if (bui_buf[0][i][j] == '5') {
					if (i == 5) {
						POSITION pos5 = { i,j };
						b_col[i][j] = 14;
						printc(padd(map_pos, pos5), bui_buf[0][i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos6 = { i,j };
						b_col[i][j] = 14;
						printc(padd(map_pos, pos6), bui_buf[0][i][j], 15, b_col[i][j]);
					}
				}
				else if (bui_buf[0][i][j] == 'P') {
					if (i == 16 || i == 17) {
						POSITION pos7 = { i,j };
						b_col[i][j] = 8;
						printc(padd(map_pos, pos7), bui_buf[0][i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos8 = { i,j };
						b_col[i][j] = 8;
						printc(padd(map_pos, pos8), bui_buf[0][i][j], 15, b_col[i][j]);
					}
				}
				else if (bui_buf[0][i][j] == 'W') {
					POSITION pos9 = { i,j };
					b_col[i][j] = 6;
					printc(padd(map_pos, pos9), bui_buf[0][i][j], 15, b_col[i][j]);
				}
				else if (bui_buf[0][i][j] == 'R') {
					POSITION pos10 = { i,j };
					b_col[i][j] = 7;
					printc(padd(map_pos, pos10), bui_buf[0][i][j], 15, b_col[i][j]);
				}
			}
			b_buf[i][j] = bui_buf[0][i][j];
		}
	}
}



// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, 15,b_col[prev.row][prev.column]);

	frontbuf[curr.row][curr.column] = b_buf[prev.row][prev.column];

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, 15,255 - b_col[curr.row][curr.column]);
	cur_loc[1][1] = b_buf[curr.row][curr.column];
}


void info_re(void) {
	POSITION pos = { 1,61 };
	printc2(padd(map_pos, pos), "실패", 15, 0);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH - 5; j++) {
			if (j == 54) {
				POSITION pos = { i ,j + 60 };
				printc(padd(map_pos, pos), r_buf[i][j + 5], COLOR_DEFAULT, 0);
				}
			else {
				POSITION pos = { i ,j + 60 };
				printc(padd(map_pos, pos), r_buf[i][j], COLOR_DEFAULT, 0);
				}
		}
	}
	hei_i = 1;
}
void sel_tp(void) {
	if (hei_i <= 16) {
		if (cur_loc[1][1] == 'R') {
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "------돌 지형------", 15, 0);
			printc2(padd(map_pos, pos2), "내릴 명령이 없습니다.",15, 0);
			hei_i++;
			char r_s[100] = "Unable to perform.";
			sys_mes();
			
		}
		else if (cur_loc[1][1] == 'P') {
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "-------장판-------", 15, 0);
			printc2(padd(map_pos, pos2), "내릴 명령이 없습니다.", 15, 0);
			hei_i++;
			
		
		}
		else if (cur_loc[1][1] == 'B') {
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "-------본진-------", 15, 0);
			printc2(padd(map_pos, pos2),"H:하베스터 생산     ", 15, 0);
			hei_i++;
			h_cre();
		}
		else {
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "-----사막 지형-----", 15, 0);
			printc2(padd(map_pos, pos2), "내릴 명령이 없습니다.", 15, 0);
			hei_i++;
			char d_s[100] = "Unable to perform.             ";
			strcpy_s(sys_message1, 100, d_s, 100);
			sys_mes();
			
		}
	}
	else if (hei_i > 16) {
		hei_i = 1;
		info_re();
	}
}

void h_cre(void) {
	char suc = '0';
	if (p_spice >= 5) {
		char h_n2[100] = "A new harvester ready            ";
		strcpy_s(sys_message1, 100, h_n2, 100);
		sys_mes();
		while (suc != 'H') {
			if (suc != 'H') {
				if (_kbhit()) {
					int key = _getch();
					if (key == 'H' || key == 'h') {
						POSITION pos = { hei_h,wid_h };
						printc(padd(map_pos, pos), 'H', 15, b_col[14][1]);
						suc = 'H';
						b_col[hei_h][wid_h] = 1;
						wid_h++;
					}
					else if (key == 27) {
						break;
					}


				}
			}
		}
	}
	else if (p_spice < 5) {
		char h_n[100] = "Not enough spice          ";
		strcpy_s(sys_message1,100, h_n,100);
		sys_mes();
		
	}
}

void sys_mes(void) {
	int ret1;
	POSITION pos5 = { hei_s - 4 ,wid_s };
	printc2(padd(map_pos, pos5), "                      ", 15, 0);

	ret1 = strncmp(sys_message1, sys_copy1, 100);

	POSITION pos1 = { hei_s-2,wid_s };
	printc2(padd(map_pos, pos1), sys_message1, 15, 0);
	if (ret1 != 0) {
		POSITION pos1 = { hei_s - 4,wid_s };
		printc2(padd(map_pos, pos1), sys_copy1, 15, 0);
		strcpy_s(sys_copy1, 100, sys_message1, 100);
	}
}
