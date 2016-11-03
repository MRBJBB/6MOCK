#pragma once
#include <windows.h>

#define BOARD_SIZE 19
#define BOARD_CENTER 9

enum Stone {
	Blank = 0,
	Player = 1,
	Opponent = 2,
	Block = 3
};

/* 주최측에서 제공한 API */

void myturn(int cnt);

/* myturn 내에서 호출되어야 함 */
void domymove(int x[], int y[], int cnt);

bool isfree(int x, int y);
bool isblock(int x, int y);
int showboard(int x, int y);


/* 
*  custom platform 용으로 사용될 API 
*  새 게임 시작시 initBoard, setBlocks 순으로 호출해주어야 함
*  AI 차례가 되면 playerTurn 함수 호출
*  상대방 차례가 끝나면 playerTurn 함수 호출 전에 updateStones로 상대방의 마지막 수 업데이트
*/
extern "C" __declspec(dllexport) void initBoard();
extern "C" __declspec(dllexport) void setBlocks(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void updateStones(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void playerTurn(int x[], int y[], int cnt);