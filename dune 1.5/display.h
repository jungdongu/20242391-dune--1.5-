/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include <string.h>
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	112
#define COLOR_RESOURCE  112




// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);

void sel_tp(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource);
void info_re(void);
void h_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void sys_mes(void);
void command_re(void);
void build_list(void);
void build_P(void);
void build_G(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void build_D(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void build_S(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void build_B(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void tower_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void h_move(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource, CURSOR cursor);
void h_select(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], RESOURCE resource, CURSOR cursor);
void b_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void F_cre(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void F_move(void);
void soldier_move(void);

#endif
