#pragma once
#include <windows.h>


#define BOARD_SIZE 19

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


/* custom platform 용으로 사용될 API */
extern "C" __declspec(dllexport) void initBoard();
extern "C" __declspec(dllexport) void setBlocks(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void updateStones(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void playerTurn(int x[], int y[], int cnt);