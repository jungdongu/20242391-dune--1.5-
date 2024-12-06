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
POSITION sand[16][58];

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char d_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char b2_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char r_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char c_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char b_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char cur_loc[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char bui_buf[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char sys_message1[100] = { 0 };
char sys_copy1[100] = { 6 };
int b_col[MAP_HEIGHT][MAP_WIDTH] = { 0 };
int hei_s = 23;
int wid_s = 5;
int hei_i = 1;
int wid_i = 78;
int hei_c = 21;
int wid_c = 78;
int hei_h = 14;
int wid_h = 2;
int build = 0; // 1:장판 2:숙소 3:창고 4:병영 5:은신처
extern int cur;
int harvest_dest_row = 1;
int harvest_dest_column = 1;
int harvest_pos_row = 1;
int harvest_pos_column = 1; h_m = 0;
int unit_row = 0, unit_column = 0;
int cur_row = 0, cur_column = 0, set = 0;
int spice_buf = 0, spice = 0, spice_max = 0, spice_population = 0, spice_population_max = 0;

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void h_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void sel_tp(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource);
void h_move(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource,CURSOR cursor);
void h_select(char mpa[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource, CURSOR cursor);
POSITION k;



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
}

void display_resource(RESOURCE resource) {
	if (spice_buf == 0) { 
		spice = resource.spice;
		spice_max = resource.spice_max;
		spice_population = resource.population;
		spice_population_max = resource.population_max;
		spice_buf = 1;
	}
	if (spice > spice_max) {
		spice = spice_max;
	}
	set_color(COLOR_RESOURCE,0);

	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		spice, spice_max,
		spice_population, spice_population_max
	);
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
				if (backbuf[i][j] == 'W') {
					POSITION pos = { i, j };
					b_col[i][j] = 6;
					printc(padd(map_pos, pos), backbuf[i][j], 15, b_col[i][j]);
					}
				else if (backbuf[i][j] == 'B') {
					if (i == 1 || i == 2) {
						POSITION pos = { i,j };
						b_col[i][j] = 12;
						printc(padd(map_pos, pos), backbuf[i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos2 = { i,j };
						b_col[i][j] = 9;
						printc(padd(map_pos, pos2), backbuf[i][j], 15, b_col[i][j]);
					}
				}
				else if (backbuf[i][j] == 'H') {
					if (i == 3) {
						POSITION pos3 = { i,j };
						b_col[i][j] = 4;
						printc(padd(map_pos, pos3), backbuf[i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos4 = { i,j };
						b_col[i][j] = 1;
						printc(padd(map_pos, pos4), backbuf[i][j], 15, b_col[i][j]);
					}
				}
				else if (1 <= backbuf[i][j] && backbuf[i][j] <= 9) {
					if (i == 5) {
						POSITION pos5 = { i,j };
						b_col[i][j] = 14;
						printc3(padd(map_pos, pos5), backbuf[i][j], 9, b_col[i][j]);
					}
					else {
						POSITION pos6 = { i,j };
						b_col[i][j] = 14;
						printc3(padd(map_pos, pos6), backbuf[i][j], 9, b_col[i][j]);
					}
				}
				else if (backbuf[i][j] == 'P') {
					if (i == 16 || i == 17) {
						POSITION pos7 = { i,j };
						b_col[i][j] = 8;
						printc(padd(map_pos, pos7), backbuf[i][j], 15, b_col[i][j]);
					}
					else {
						POSITION pos8 = { i,j };
						b_col[i][j] = 8;
						printc(padd(map_pos, pos8), backbuf[i][j], 15, b_col[i][j]);
					}
				}
			
				else if (backbuf[i][j] == 'R') {
					POSITION pos10 = { i,j };
					b_col[i][j] = 7;
					printc(padd(map_pos, pos10), backbuf[i][j], 15, b_col[i][j]);
				}
				else {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT, 0);
				}
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
	if (set == 0) {
		POSITION pos3 = { hei_c ,wid_c + 1 };
		printc2(padd(map_pos, pos3), "B:Build", 15, 0);
		set++;
	}
}


// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	if (b_col[prev.row][prev.column] == 6) {
		printc(padd(map_pos, prev), ch, 15, 0);
	}
	else if (cur == 1) {
		printc(padd(map_pos, prev), ch, 15, b_col[prev.row][prev.column]);
		prev.column++;
		printc(padd(map_pos, prev), ch, 15, b_col[prev.row][prev.column]);
		prev.row++;
		printc(padd(map_pos, prev), ch, 15, b_col[prev.row][prev.column]);
		prev.column--;
		printc(padd(map_pos, prev), ch, 15, b_col[prev.row][prev.column]);
	}
	else {
		printc(padd(map_pos, prev), ch, 15, b_col[prev.row][prev.column]);
	
	}
	
	frontbuf[curr.row][curr.column] = backbuf[prev.row][prev.column];
	ch = frontbuf[curr.row][curr.column];

	if (b_col[prev.row][prev.column] == 6) {
		printc(padd(map_pos, curr), ch, 15, 0);
	}
	else if (cur == 1){
		cur_row = curr.row;
		cur_column = curr.column;
		printc(padd(map_pos, curr), ch, 15, 255 - b_col[curr.row][curr.column]);
		curr.column++;
		printc(padd(map_pos, curr), ch, 15, 255 - b_col[curr.row][curr.column]);
		curr.row++;
		printc(padd(map_pos, curr), ch, 15, 255 - b_col[curr.row][curr.column]);
		curr.column--;
		printc(padd(map_pos, curr), ch, 15, 255 - b_col[curr.row][curr.column]);
	}
	else {
		printc(padd(map_pos, curr), ch, 15, 255 - b_col[curr.row][curr.column]);
	}
	cur_loc[1][1] = backbuf[curr.row][curr.column];
	unit_row = curr.row, unit_column = curr.column;
	
}


void info_re(void) {
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
void command_re(void) {
	for (int i = 0; i < MAP_HEIGHT - 10; i++) {
		for (int j = 0; j < MAP_WIDTH - 5; j++) {
			if (i == 0) {
				POSITION pos = { i + 18 ,j + 60 };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT, 0);
			}
			else if (j == 54) {
				POSITION pos = { i + 18 ,j + 60 };
				printc(padd(map_pos, pos), backbuf[i][j + 5], COLOR_DEFAULT, 0);
			}
			else {
				POSITION pos = { i + 18 ,j + 60 };
				printc(padd(map_pos, pos), backbuf[i + 10][j], COLOR_DEFAULT, 0);
			}
		}
	}
	set = 0;
}
void sel_tp(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource) {
	if (hei_i <= 16) {
		if (cur_loc[1][1] == 'R') {
			command_re();
			set = 1;
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "------돌 지형------", 15, 0);
			printc2(padd(map_pos, pos2), "내릴 명령이 없습니다.",15, 0);
			hei_i++;
			char r_s[100] = "Unable to perform.";
			sys_mes();
			
		}
		else if (cur_loc[1][1] == 'P') {
			command_re();
			set = 1;
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "-------장판-------", 15, 0);
			printc2(padd(map_pos, pos2), "내릴 명령이 없습니다.", 15, 0);
			hei_i++;
			
		
		}
		else if (cur_loc[1][1] == 'B') {
			command_re();
			set = 1;
			POSITION pos = { hei_i,wid_i };
			POSITION pos2 = { hei_c , wid_c };
			printc2(padd(map_pos, pos), "-------본진-------", 15, 0);
			printc2(padd(map_pos, pos2),"H:하베스터 생산.", 15, 0);
			hei_i++;
			h_cre(map, resource);
		}
		else if(cur_loc[1][1] == ' '){
			command_re();
			set = 1;
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
void h_select(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],RESOURCE resource, CURSOR cursor) {
	if (cur_loc[1][1] == 'H') {
		command_re();
		set = 1;
		harvest_pos_row = unit_row;
		harvest_pos_column = unit_column;
		POSITION pos = { hei_i,wid_i };
		POSITION pos2 = { hei_c , wid_c };
		printc2(padd(map_pos, pos), "이름:하베스터,체력:15", 15, 0);
		pos.row++;
		printc2(padd(map_pos, pos), "■■■■■", 15, 0);
		pos.row++;
		printc2(padd(map_pos, pos), "■□■□■", 15, 0);
		pos.row++;;
		printc2(padd(map_pos, pos), "■■■■■", 15, 0);
		pos.row++;
		printc2(padd(map_pos, pos), "■□□□■", 15, 0);
		pos.row++;
		printc2(padd(map_pos, pos), "■■■■■", 15, 0);
		printc2(padd(map_pos, pos2), "H:Harvest(수확),M:move(이동)", 15, 0);
		hei_i += 6;
		h_move(map,resource, cursor);
		}
}
void build_list(void) {
	command_re();
	set = 1;
	POSITION pos = { hei_c-1,wid_c - 15 };
	printc2(padd(map_pos, pos), "장판(P:Plate) 숙소(D:Dormitory) 창고(G:Garage)",15,0);
	POSITION pos2 = { hei_c + 1,wid_c - 15 };
	printc2(padd(map_pos, pos2), "병영(A:Barracks) 은신처(R:Shelter)", 15, 0);

}
void h_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	char suc = '0';
		while (suc != 'H') {
			if (suc != 'H') {
				if (_kbhit()) {
					int key = _getch();
					if (key == 'H' || key == 'h') {
						if (spice >= 5) {
							char h_n2[100] = "A new harvester ready            ";
							strcpy_s(sys_message1, 100, h_n2, 100);
							sys_mes();
							POSITION pos = { hei_h,wid_h };
							suc = 'H';
							b_col[hei_h][wid_h] = 1;
							spice -= 5;
							map[1][hei_h][wid_h] = 'H';
							wid_h++;
							break;
						}
						else if(spice < 5){
							char h_n[100] = "Not enough spice          ";
							strcpy_s(sys_message1, 100, h_n, 100);
							sys_mes();
							break;
						}
					}
					else if (key == 27) {
						break;
					}


				}
			}
		}
	}
void sys_mes(void) {
	int ret1;
	POSITION pos5 = { hei_s - 4 ,wid_s };
	printc2(padd(map_pos, pos5), "                      ", 15, 0);

	ret1 = strncmp(sys_message1, sys_copy1, 100);

	POSITION pos1 = { hei_s-1,wid_s };
	printc2(padd(map_pos, pos1), sys_message1, 15, 0);
	if (ret1 != 0) {
		POSITION pos1 = { hei_s - 3,wid_s };
		printc2(padd(map_pos, pos1), sys_copy1, 15, 0);
		strcpy_s(sys_copy1, 100, sys_message1, 100);
	}
	else if (ret1 == 0) {
		POSITION pos1 = { hei_s - 3,wid_s };
		printc2(padd(map_pos, pos1), sys_copy1, 15, 0);
	}
}

void build_P(void) {
	build = 1;
	cur = 1;
}
void build_G(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	build = 3;
	cur = 1;
}
void build_D(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	build = 2;
	cur = 1;
}
void build_S(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	build = 5;
	cur = 1;
}
void build_B(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	build = 4;
	cur = 1;
}
void tower_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]){
	if (cur_loc[1][1] == ' ') {
		if (build == 1) {
			if (spice >= 1) {
				char b_s[100] = "장판 건설완료!                   ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				POSITION pos = { cur_row,cur_column };
				map[1][cur_row][cur_column] = 'P';
				cur_row++;
				map[1][cur_row][cur_column] = 'P';
				cur_column++;
				map[1][cur_row][cur_column] = 'P';
				cur_row--;
				map[1][cur_row][cur_column] = 'P';
				spice -= 1;
				cur = 0;
			}
			else if (spice < 1) {
				char b_s[100] = "스파이스 부족                     ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				cur = 0;
			}
		}
	}
	if (cur_loc[1][1] == 'P') {
		if (build == 2) {
			if (spice >= 2) {
				char b_s[100] = "숙소 건설완료!                   ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				POSITION pos = { cur_row,cur_column };
				map[1][cur_row][cur_column] = 'D';
				cur_row++;
				map[1][cur_row][cur_column] = 'D';
				cur_column++;
				map[1][cur_row][cur_column] = 'D';
				cur_row--;
				map[1][cur_row][cur_column] = 'D';
				spice -= 2;
				spice_population_max += 10;
				cur = 0;
			}
			else if (spice < 2) {
				char b_s[100] = "스파이스 부족                     ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				cur = 0;
			}
		}
		if (build == 3) {
			if (spice >= 4) {
				char b_s[100] = "창고 건설완료!                   ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				POSITION pos = { cur_row,cur_column };
				map[1][cur_row][cur_column] = 'G';
				cur_row++;
				map[1][cur_row][cur_column] = 'G';
				cur_column++;
				map[1][cur_row][cur_column] = 'G';
				cur_row--;
				map[1][cur_row][cur_column] = 'G';
				spice_max += 10;
				spice -= 4;
				cur = 0;
			}
			else if (spice < 4) {
				char b_s[100] = "스파이스 부족                     ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				cur = 0;
			}
		}
		if (build == 4) {
			if (spice >= 4) {
				char b_s[100] = "병영 건설완료!                   ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				POSITION pos = { cur_row,cur_column };
				map[1][cur_row][cur_column] = 'B';
				cur_row++;
				map[1][cur_row][cur_column] = 'B';
				cur_column++;
				map[1][cur_row][cur_column] = 'B';
				cur_row--;
				map[1][cur_row][cur_column] = 'B';
				spice -= 4;
				cur = 0;
			}
			else if (spice < 4) {
				char b_s[100] = "스파이스 부족                     ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				cur = 0;
			}
		}
		if (build == 5) {
			if (spice >= 5) {
				char b_s[100] = "은신처 건설완료!                   ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				POSITION pos = { cur_row,cur_column };
				map[1][cur_row][cur_column] = 'S';
				cur_row++;
				map[1][cur_row][cur_column] = 'S';
				cur_column++;
				map[1][cur_row][cur_column] = 'S';
				cur_row--;
				map[1][cur_row][cur_column] = 'S';
				spice -= 5;
				cur = 0;
			}
			else if (spice < 5) {
				char b_s[100] = "스파이스 부족                     ";
				strcpy_s(sys_message1, 100, b_s, 100);
				sys_mes();
				cur = 0;
			}
		}
	}
}
void h_move(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource, CURSOR cursor) {
	if (cur != 2) {
		while (1) {
			KEY key = get_key();
			if (key == k_habester) {
				break;
			}
		}
	}
	cur = 2;
	if (cur_loc[1][1] == 1 || cur_loc[1][1] == 2 || cur_loc[1][1] == 3 \
		|| cur_loc[1][1] == 4 || cur_loc[1][1] == 5 || cur_loc[1][1] == 6 \
		|| cur_loc[1][1] == 7 || cur_loc[1][1] == 8 || cur_loc[1][1] == 9) {
		harvest_dest_row =unit_row;
		harvest_dest_column = unit_column;
		cur = 0;
		h_m = 1;
	}
}

