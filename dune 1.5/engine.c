//20242391 정동우
//1) ~ 5) 까지 했습니다.
//보너스는 하지 않았습니다.


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
void harvest_obj_move(void);
void Soldier_obj_move(void);
void Fremen_obj_move(void);
POSITION sample_obj_next_position(void);
POSITION sand_next_position(void);
POSITION sand2_next_position(void);
POSITION sand_new_dest(void);
POSITION sand2_new_dest(void);
POSITION harvest_next_position(void);
POSITION Soldier_next_position(void);
POSITION Fremen_next_position(void);
void sand_obj_move(void);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
int p = 1;
int ss = 1;
int ff = 1;
int close_harvest = 1;
extern int cur = 0;
extern int harvest_dest_row;
extern int harvest_dest_column;
extern int harvest_pos_row;
extern int harvest_pos_column;
extern int Soldier_dest_row;
extern int Soldier_dest_column;
extern int Soldier_pos_row;
extern int Soldier_pos_column;
extern int Fremen_dest_row;
extern int Fremen_dest_column;
extern int Fremen_pos_row;
extern int Fremen_pos_column;
extern int h_m;
extern int s_m;
extern int f_m;
extern int spice, spice_max, spice_population, spice_population_max;
RESOURCE resource = { 
	.spice = 50,
	.spice_max = 50,
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

OBJECT_SAMPLE Fremen = {
	.repr = 'F',
	.speed = 300,
	.next_move_time = 400,
	.hp = 25,
	.attack = 15,
	.attack_time = 200,
	.population = 2,
};

OBJECT_SAMPLE Soldier = {
	.repr = 'S',
	.speed = 300,
	.next_move_time = 1000,
	.hp = 15,
	.attack = 5,
	.attack_time = 800,
	.population = 1,
};
OBJECT_SAMPLE Harvest = {
	.repr = 'H',
	.next_move_time = 2000,
	.hp = 70,
	.population = 5,
};

char s_buf[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char copy_buf [MAP_HEIGHT][MAP_WIDTH] = { 0 };
char sandworm[MAP_HEIGHT][MAP_WIDTH] = { 0 };
const POSITION map_pos2 = { 1, 0 };
int p_h_len = 0;
int a_h_len = 0;
POSITION P_h_location = { 0,0 };
POSITION A_h_location = { 0,0 };
POSITION soldier_location = { 0,0 };
POSITION Fremen_location = { 0,0 };

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
				if (cur == 0) { 
					sel_tp(map, resource);
					h_select(map,resource,cursor);
				}
				else if (cur == 1) {
					tower_cre(map);
				}
				else if (cur == 2) {
					h_move(map, resource, cursor);
				}
				else if (cur == 3) {
					soldier_move();
				}
				else if (cur == 4) {
					F_move();
				}
				break;
			case k_re:
				info_re();
				command_re();
				break;
			case k_build:
				build_list();
				break;
			case k_Plate:
				build_P();
				break;
			case k_Garge:
				build_G(map);
				break;
			case k_Barracks:
				build_B(map);
				break;
			case k_Dormitory:
				build_D(map);
				break;
			case k_Shelter:
				build_S(map);
				break;
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sand_obj_move();
		if (h_m == 1) {
			if (p == 1) {
				Harvest.pos.row = harvest_pos_row;
				Harvest.pos.column = harvest_pos_column;
				Harvest.dest.row = harvest_dest_row;
				Harvest.dest.column = harvest_dest_column;
				close_harvest = 1;
				Harvest.next_move_time = sys_clock;
				p = 0;

			}
			harvest_obj_move();
			if ((Harvest.pos.row == sand.pos.row && Harvest.pos.column == sand.pos.column) || \
				Harvest.pos.row == sand2.pos.row && Harvest.pos.column == sand2.pos.column) {
				h_m = 0;
			}
		}
		if (s_m == 1) {
			if (ss == 1) {
				Soldier.pos.row = Soldier_pos_row;
				Soldier.pos.column = Soldier_pos_column;
				Soldier.dest.row = Soldier_dest_row;
				Soldier.dest.column = Soldier_dest_column;
				Soldier.next_move_time = sys_clock;
				ss = 0;
			}
			Soldier_obj_move();
		}
		if (f_m == 1) {
			if (ff == 1) {
				Fremen.pos.row = Fremen_pos_row;
				Fremen.pos.column = Fremen_pos_column;
				Fremen.dest.row = Fremen_dest_row;
				Fremen.dest.column = Fremen_dest_column;
				Fremen.next_move_time = sys_clock;
				ff = 0;

			}
			Fremen_obj_move();
		}
		
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

	printf("\n");

	printf(" #######  ##     ##  ########  ######## \n");
	printf("##     ## ##     ##  ##        ##     ## \n");
	printf("##     ## ##     ##  ##        ##     ## \n");
	printf("##     ## ##     ##  ########  ########  \n");
	printf("##     ## ##     ##  ##        ##     ##  \n");
	printf("##     ##  ##   ##   ##        ##      ## \n");
	printf(" #######    #####    ########  ##       ##\n");
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
			else if ((i == 5 && j == 58) || (i == 10 && j == 1)) {
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
POSITION harvest_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(Harvest.dest, Harvest.pos);
	DIRECTION dir;
	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if ((diff.row == 1 || diff.row == -1 || diff.row == 0) && \
		(diff.column == 0 || diff.column == 1 || diff.column == -1)) {
		if (Harvest.dest.row == harvest_pos_row && Harvest.dest.column == harvest_pos_column) {
			POSITION new_dest = { harvest_dest_row , harvest_dest_column };
			Harvest.dest = new_dest;
			if (close_harvest == 0) {
				h_m = 0;
			}
		}
		else {
			POSITION new_dest = { harvest_pos_row , harvest_pos_column };
			Harvest.dest = new_dest;
			map[1][harvest_dest_row][harvest_dest_column]--;
			int spice_random = rand() % 3 + 2;
			spice += spice_random;
			if (map[1][harvest_dest_row][harvest_dest_column] == 0) {
				map[1][harvest_dest_row][harvest_dest_column] = ' ';
				POSITION new_dest2 = { harvest_pos_row, harvest_pos_column };
				Harvest.dest = new_dest2;
				close_harvest = 0;
			}
		}
		return Harvest.pos;
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
	POSITION next_pos = pmove(Harvest.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0 || (map[1][next_pos.row][next_pos.column] < 10 && map[1][next_pos.row][next_pos.column] >= 1)) {
		if (map[1][next_pos.row][next_pos.column] < 10 && map[1][next_pos.row][next_pos.column] >= 1) {
			return next_pos;
		}
		return next_pos;
	}
	else {
		return Harvest.pos;  // 제자리
	}
}

POSITION Fremen_next_position(void) {
	POSITION diff = psub(Fremen.dest, Fremen.pos);
	DIRECTION dir;

	if (diff.row == 0 && diff.column == 0) {
		if (Fremen.dest.row == Fremen_pos_row && Fremen.dest.column == Fremen_pos_column) {
			POSITION new_dest = { Fremen_dest_row, Fremen_dest_column};
			Fremen.dest = new_dest;
		}
		else {
			POSITION new_dest = { Fremen_pos_row, Fremen_pos_column };
			Fremen.dest = new_dest;
		}
		return Fremen.pos;
	}

	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(Fremen.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return Fremen.pos;  // 제자리
	}
}
POSITION Soldier_next_position(void) {
	POSITION diff = psub(Soldier.dest, Soldier.pos);
	DIRECTION dir;

	if (diff.row == 0 && diff.column == 0) {
		if (Soldier.dest.row == Soldier_pos_row && Soldier.dest.column == Soldier_pos_column) {
			POSITION new_dest = { Soldier_dest_row, Soldier_dest_column };
			Soldier.dest = new_dest;
		}
		else {
			POSITION new_dest = { Soldier_pos_row, Soldier_pos_column };
			Soldier.dest = new_dest;
		}
		return Soldier.pos;
	}

	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(Soldier.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return Soldier.pos;  // 제자리
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
				P_h_location.row = i;
				P_h_location.column = j;
			}
			if (map[1][i][j] == 'F'){
				Fremen_location.row = i;
				Fremen_location.column = j;
			}
			if (map[1][i][j] == 'S') {
				Fremen_location.row = i;
				Fremen_location.column = j;
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
				P_h_location.row = i;
				P_h_location.column = j;

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

void harvest_obj_move(void) {
	if (sys_clock <= Harvest.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}
	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][Harvest.pos.row][Harvest.pos.column] = -1;
	Harvest.pos = harvest_next_position();
	map[1][Harvest.pos.row][Harvest.pos.column] = Harvest.repr;
	Harvest.next_move_time += 2000;
}
void Soldier_obj_move(void) {
	if (sys_clock <= Soldier.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}
	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][Soldier.pos.row][Soldier.pos.column] = -1;
	Soldier.pos = Soldier_next_position();
	map[1][Soldier.pos.row][Soldier.pos.column] = Soldier.repr;
	Soldier.next_move_time += 1000;
}
void Fremen_obj_move(void) {
	if (sys_clock <= Fremen.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}
	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][Fremen.pos.row][Fremen.pos.column] = -1;
	Fremen.pos = Fremen_next_position();
	map[1][Fremen.pos.row][Fremen.pos.column] = Fremen.repr;
	Fremen.next_move_time += 400;
}
void sand_obj_move(void) {

	if (t.next_move_time <= sys_clock) {
		int random = rand() % 9 + 1;
		int random2 = rand() % 9 + 1;
		map[1][sand.pos.row][sand.pos.column + 1] = random;
		map[1][sand2.pos.row][sand2.pos.column + 1] = random2;
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
