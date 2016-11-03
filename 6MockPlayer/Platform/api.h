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

/* ���������� ������ API */

void myturn(int cnt);

/* myturn ������ ȣ��Ǿ�� �� */
void domymove(int x[], int y[], int cnt);

bool isfree(int x, int y);
bool isblock(int x, int y);
int showboard(int x, int y);


/* 
*  custom platform ������ ���� API 
*  �� ���� ���۽� initBoard, setBlocks ������ ȣ�����־�� ��
*  AI ���ʰ� �Ǹ� playerTurn �Լ� ȣ��
*  ���� ���ʰ� ������ playerTurn �Լ� ȣ�� ���� updateStones�� ������ ������ �� ������Ʈ
*/
extern "C" __declspec(dllexport) void initBoard();
extern "C" __declspec(dllexport) void setBlocks(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void updateStones(int x[], int y[], int cnt);
extern "C" __declspec(dllexport) void playerTurn(int x[], int y[], int cnt);