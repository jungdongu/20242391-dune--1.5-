#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include "io.h"
#include "display.h"


void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);
POSITION sand_next_position(void);
POSITION sand2_next_position(void);
POSITION sand_new_dest(void);
POSITION sand2_new_dest(void);
void sand_obj_move(void);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

RESOURCE resource = { 
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};
OBJECT_SAMPLE sand = {
	.pos = {4,12},
	.dest = {4,12},
	.repr = 'W',
	.speed = 300,
	.next_move_time = 1000,
};

OBJECT_SAMPLE sand2 = {
	.pos = {14,39},
	.dest = {14,39},
	.repr = 'W',
	.speed = 300,
	.next_move_time = 3000,
};

OBJECT_SAMPLE t = {
	.pos = {14,39},
	.dest = {14,39},
	.repr = 'W',
	.speed = 300,
	.next_move_time = 10000,
};
char s_buf[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char copy_buf [MAP_HEIGHT][MAP_WIDTH] = { 0 };
char sandworm[MAP_HEIGHT][MAP_WIDTH] = { 0 };
const POSITION map_pos2 = { 1, 0 };
int p_h_len = 0;
int a_h_len = 0;
POSITION P_h_location = { 0,0 };
POSITION A_h_location = { 0,0 };


/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));
	
	init();
	intro();
	display(resource, map, cursor);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			cursor_move(ktod(key));
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_tp: 
				sel_tp(map);
				break;
			case k_re:
				info_re();
				break;
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();
		sand_obj_move();

		// 화면 출력
		display(resource, map, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	POSITION pos = { 10,15 };
	printf("#####   #      #  #########  ##      #      #           ########        \n");
	printf("#    #  #      #  ##         # #     #     ##           ##        \n");
	printf("#    #  #      #  ##         #  #    #    # #           ##        \n");
	printf("#    #  #      #  #########  #   #   #      #           #######        \n");
	printf("#    #  #      #  ##         #    #  #      #                  #   \n");
	printf("#    #   #    #   ##         #     # #      #       ##         #    \n");
	printf("#####     ####    #########  #      ##  #########   ##  #######       \n");
	Sleep(2000);
	system("cls");
}

void outro(void) {
	system("cls");
	printc(map_pos2, ' ', 15, 0);
	printf(" #####        ###       ###        ###  ######### \n");
	printf("##   ##      ## ##       ## ##    ## ##  ##        \n");
	printf("##          ##   ##      ##  ## ##   ##  ##         \n");
	printf("##  ###    ##     ##     ##   ###    ##  #########  \n");
	printf("##   ##   ###########    ##          ##  ##         \n");
	printf("##   ##  ##         ##   ##          ##  ##         \n");
	printf(" #####  ##           ##  ##          ##  #########  \n");

	printf("\n"); // 줄 간격 추가

	printf(" #######  ##     ##  ########  ######## \n");
	printf("##     ## ##     ##  ##        ##     ## \n");
	printf("##     ## ##     ##  ##        ##     ## \n");
	printf("##     ## ##     ##  ########  ########  \n");
	printf("##     ## ##     ##  ##        ##     ##  \n");
	printf("##     ##  ##   ##   ##        ##      ## \n");
	printf(" #######    #####    ########  ##       ##\n");
	printf("exiting...\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH-1; j++) {
			map[0][i][j] = ' ';
		}
	}



	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if ((1 <= i && i <= 2) && (57 <= j && j <= 58)) {
				map[1][i][j] = 'B';
			}
			else if ((i >= 15 && i <= 16) && (j >= 1 && j <= 2)) {
				map[1][i][j] = 'B';
			}
			else if ((i == 3 && j == 58) || (i == 14 && j == 1)) {
				map[1][i][j] = 'H';
			}
			else if (((i >= 1 && i <= 2) && (j >= 55 && j <= 56)) || ((i >= 15 && i <= 16) && (j >= 3 && j <= 4))) {
				map[1][i][j] = 'P';
			}
			else if ((i == 5 && j == 58) || (i == 12 && j == 1)) {
				map[1][i][j] = 5;
			}
			else if ((i == 5 && j == 48) || (i == 14 && j == 22) || (i == 15 && j == 54)) {
				map[1][i][j] = 'R';
			}
			else if (((i >= 4 && i <= 5) && (j >= 33 && j <= 34)) || ((i >= 14 && i <= 15) && (j >= 35 && j <= 36))) {
				map[1][i][j] = 'R';
			}
		}
	}


	// object sample
	
	map[1][obj.pos.row][obj.pos.column] = 'o';
	map[1][sand.pos.row][sand.pos.column] = 'W';
	map[1][sand2.pos.row][sand2.pos.column] = 'W';
	
	
}

// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);

	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}
	
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		
		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

POSITION sand_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(sand.dest, sand.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		sand_new_dest();
		return sand.pos;
	}

	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

		// validation check
		// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
		// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
		POSITION next_pos = pmove(sand.pos, dir);
		if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
			1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
			map[1][next_pos.row][next_pos.column] < 0 || map[1][next_pos.row][next_pos.column] == 'H' \
			|| map[1][next_pos.row][next_pos.column] == 'R') {

			return next_pos;
		}
		else {
			return sand.pos;  // 제자리
		}
		
}

POSITION sand2_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(sand2.dest, sand2.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		sand2_new_dest();
		return sand2.pos;
	}

	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(sand2.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0 || map[1][next_pos.row][next_pos.column] == 'H' \
		|| map[1][next_pos.row][next_pos.column] == 'R') {

		return next_pos;
	}
	else {
		return sand2.pos;  // 제자리
	}

}
POSITION sand_new_dest(void) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[1][i][j] == 'H') {
				if (i == 3) {
					A_h_location.row = i;
					A_h_location.column = j;
				}
				else if (i == 14) {
					P_h_location.row = i;
					P_h_location.column = j;
				}

			}
			copy_buf[i][j] = map[0][i][j];
		}
	}


	POSITION P_H = psub(P_h_location, sand.pos);
	p_h_len = root(P_H.row, P_H.column);
	POSITION A_H = psub(A_h_location, sand.pos);
	a_h_len = root(A_H.row, A_H.column);


	if (p_h_len <= a_h_len) {
		if (p_h_len == 0) {
			map[1][P_h_location.row][P_h_location.column] = ' ';
			P_h_location.row = 1000;
			P_h_location.column = 1000;
			sand.dest.row = A_h_location.row;
			sand.dest.column = A_h_location.column;
			return sand.dest;
		}
		sand.dest.row = P_h_location.row;
		sand.dest.column = P_h_location.column;
		return sand.dest;
	}


	else {
		if (a_h_len == 0) {
			map[1][A_h_location.row][A_h_location.column] = ' ';
			A_h_location.row = 1000;
			A_h_location.column = 1000;
			sand.dest.row = P_h_location.row;
			sand.dest.column = P_h_location.column;
			return sand.dest;
		}
		sand.dest.row = A_h_location.row;
		sand.dest.column = A_h_location.column;
		return sand.dest;
		if (map[1][sand.pos.row][sand.pos.column] == 'R') {
			map[1][sand.pos.row][sand.pos.column] == ' ';
		}
	}
}
POSITION sand2_new_dest(void) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[1][i][j] == 'H') {
				if (i == 3) {
					A_h_location.row = i;
					A_h_location.column = j;
				}
				else if (i == 14) {
					P_h_location.row = i;
					P_h_location.column = j;
				}

			}
			copy_buf[i][j] = map[0][i][j];
		}
	}


	POSITION P_H = psub(P_h_location, sand2.pos);
	p_h_len = root(P_H.row, P_H.column);
	POSITION A_H = psub(A_h_location, sand2.pos);
	a_h_len = root(A_H.row, A_H.column);


	if (p_h_len <= a_h_len) {
		if (p_h_len == 0) {
			map[1][P_h_location.row][P_h_location.column] = ' ';
			P_h_location.row = 1000;
			P_h_location.column = 1000;
			sand2.dest.row = A_h_location.row;
			sand2.dest.column = A_h_location.column;
			return sand2.dest;
		}
		sand2.dest.row = P_h_location.row;
		sand2.dest.column = P_h_location.column;
		return sand2.dest;
	}


	else {
		if (a_h_len == 0) {
			map[1][A_h_location.row][A_h_location.column] = ' ';
			A_h_location.row = 1000;
			A_h_location.column = 1000;
			sand2.dest.row = P_h_location.row;
			sand2.dest.column = P_h_location.column;
			return sand2.dest;
		}
		sand2.dest.row = A_h_location.row;
		sand2.dest.column = A_h_location.column;
		return sand2.dest;
	}
	if (map[1][sand2.pos.row][sand2.pos.column] == 'R') {
		map[1][sand2.pos.row][sand2.pos.column] == ' ';
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}
	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;
	obj.next_move_time = sys_clock + obj.speed;
}

void sand_obj_move(void) {

	if (t.next_move_time <= sys_clock) {
		int random = rand() % 9 + 1;
		map[1][sand.pos.row][sand.pos.column + 1] = random;
		map[1][sand2.pos.row][sand2.pos.column + 1] = random;
		t.next_move_time += 10000;

	}
	if (sys_clock <= sand.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}


	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][sand.pos.row][sand.pos.column] = -1;
	map[1][sand2.pos.row][sand2.pos.column] = -1;

	sand.dest = sand_new_dest();
	sand2.dest = sand_new_dest();

	sand.pos = sand_next_position();
	sand2.pos = sand2_next_position();
	map[1][sand.pos.row][sand.pos.column] = sand.repr;
	map[1][sand2.pos.row][sand2.pos.column] = sand2.repr;
	

	sand.next_move_time += 1000;
	sand2.next_move_time += 1000;
}
