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
char b_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void d_build(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);

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
	d_build(map);
	//
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
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
				POSITION pos = {i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
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
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
				}
				else {
					POSITION pos = { i + 18,j };
					printc(padd(map_pos, pos), backbuf[i+10][j], COLOR_DEFAULT);
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
					printc(padd(map_pos, pos), backbuf[i][j+5], COLOR_DEFAULT);
				}
				else {
					POSITION pos = { i ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
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
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
				}
				else if (j == 54) {
					POSITION pos = { i + 18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j + 5], COLOR_DEFAULT);
				}
				else {
					POSITION pos = { i+18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i + 10][j], COLOR_DEFAULT);
				}
			}
			c_buf[i][j] = backbuf[i][j];
		}
	}
}

void d_build(char buil[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (k == 0) {
					if (i == 12 && buil[k][i][j] != b_buf[i][j]) {
						POSITION pos = { 1,57 };
						printc(padd(map_pos, pos), ' ', 15,i);
						pos.column++;
						printc(padd(map_pos, pos), ' ', 15,i);
						POSITION pos2 = { 2,57 };
						printc(padd(map_pos, pos2), 'B', 15,i);
						pos2.column++;
						printc(padd(map_pos, pos2), ' ', 15,i);
						pos2.row++;
						printc(padd(map_pos, pos2), 'H', 15,i);
					}
					else if (i == 9 && buil[k][i][j] != b_buf[i][j]) {

					}
					b_buf[i][j] = buil[k][i][j];
				}
			}
		}
	} 
}





// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);

}
