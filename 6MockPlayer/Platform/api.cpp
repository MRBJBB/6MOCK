#include <string.h>

#include "api.h"
#include "../Minmax/minmax_node.h"

using namespace std;

char GameBoard[BOARD_SIZE][BOARD_SIZE];
Point PlayerMovement[2];
Point OpponentMovement[2];

int PlayerMoveCnt; // 0이면 새로운 게임 시작
int OpponentMoveCnt;

void initBoard() {
	memset(GameBoard, Blank, sizeof(GameBoard));
	PlayerMoveCnt = 0;
}

void domymove(int x[], int y[], int cnt) {
	PlayerMoveCnt = cnt;
	for (int i = 0; i < cnt; i++) {
		GameBoard[x[i]][y[i]] = Player;
		PlayerMovement[i].x = x[i];
		PlayerMovement[i].y = y[i];
	}
}

void setBlocks(int x[], int y[], int cnt) {
	for (int i = 0; i < cnt; i++) 
		GameBoard[x[i]][y[i]] = Block;
}

void updateStones(int x[], int y[], int cnt) {
	OpponentMoveCnt = cnt;
	for (int i = 0; i < cnt; i++) {
		GameBoard[x[i]][y[i]] = Opponent;
		OpponentMovement[i].x = x[i];
		OpponentMovement[i].y = y[i];
	}
}

void playerTurn(int x[], int y[], int cnt) {
	myturn(cnt);
	for (int i = 0; i < cnt; i++) {
		x[i] = PlayerMovement[i].x;
		y[i] = PlayerMovement[i].y;
	}
}

bool isfree(int x, int y) {
	return GameBoard[x][y] == Blank;
}

bool isblock(int x, int y) {
	return GameBoard[x][y] == Block;
}

int showboard(int x, int y) {
	return GameBoard[x][y];
}