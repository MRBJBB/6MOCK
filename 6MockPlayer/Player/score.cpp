#include <assert.h>
#include <initializer_list>

#include "score.h"

using namespace std;

/* Read Only */
extern char GameBoard[BOARD_SIZE][BOARD_SIZE];

float ScoreBoard[BOARD_SIZE][BOARD_SIZE];
char ActiveArea[BOARD_SIZE][BOARD_SIZE];
char PlayerBoard[BOARD_SIZE][BOARD_SIZE];

int dirDeltaX[4] = {1, 0, 1, 1};
int dirDeltaY[4] = {0, 1, -1, 1};

void initPlayerBoard() {
	memcpy(PlayerBoard, GameBoard, sizeof(PlayerBoard));

	// 블록이 있는 위치
	int bcnt, bx[6], by[6];
	bcnt = getBlocks(bx, by);

	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			// 중심에 가까울수록 유리
			int dist = maxNorm(x, y, BOARD_CENTER, BOARD_CENTER);
			ScoreBoard[x][y] = INITIAL_SCORE_FACTOR * (1.0f - (float)dist / (float)BOARD_CENTER);

			// 블록에 가까울수록 불리
			for (int i = 0; i < bcnt; i++) {
				dist = maxNorm(x, y, bx[i], by[i]);
				int penalty = max(BLOCK_PENALTY_DIST - dist, 0);
				ScoreBoard[x][y] *= (BLOCK_PENALTY_FACTOR * (float)penalty);
			}
		}
	}
}

int getBlocks(int bx[], int by[]) {
	int cnt = 0;
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++) {
			if (GameBoard[x][y] == Block) {
				bx[cnt] = x; by[cnt] = y;
				cnt++;
			}
		}
	return cnt;
}

// 해당 위치에 돌을 놓았을 때의 점수
float score(int x[], int y[], int cnt, Stone stone) {
	float res = 0.0f;
	
	for (int i = 0; i < cnt; i++) {
		assert(PlayerBoard[x[i]][y[i]] == Blank);
		PlayerBoard[x[i]][y[i]] = stone;
	}

	for (int i = 0; i < cnt; i++) {

	}

	for (int i = 0; i < cnt; i++) 
		PlayerBoard[x[i]][y[i]] = Blank;

	return res;
}



void getLineInfo(int x, int y, Stone stone, Direction dir, int& len, int& bound, int& span) {
	// bound : 막혀있는 방향의 수 (0, 1, 2)
	// span : 이어 놓을 수 있는 빈칸의 수 (0, ..., 6)
	int xx = x, yy = y, dx = dirDeltaX[dir], dy = dirDeltaY[dir];

	len = 0, bound = 0, span = 0;
	while (isInbound(xx, yy) && PlayerBoard[xx][yy] == stone) {
		len++;
		xx += dx, yy += dy;
	}
	if (!isInbound(xx, yy) || PlayerBoard[xx][yy] != Blank)
		bound++;
	else {
		while (isInbound(xx, yy) && PlayerBoard[xx][yy] == Blank) {
			span++;
			xx += dx, yy += dy;
		}
	}

	xx = x, yy = y, dx *= -1, dy *= -1;
	while (isInbound(xx, yy) && PlayerBoard[xx][yy] == stone) {
		len++;
		xx += dx, yy += dy;
	}
	if (!isInbound(xx, yy) || PlayerBoard[xx][yy] != Blank)
		bound++;
	else {
		while (isInbound(xx, yy) && PlayerBoard[xx][yy] == Blank) {
			span++;
			xx += dx, yy += dy;
		}
	}
	
	len--;
}

/*
inline bool isInSameLine(int x1, int y1, int x2, int y2) {
	if (x1 == x2)

	else if (y1 == y2);
	else if (x1 + y1 == x2 + y2);
	else if (abs(x1 - y1) == abs(x2 - y2));

}
*/
inline bool isInbound(int x, int y) {
	return x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0;
}
inline int maxNorm(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
}
inline Stone getOpponent(Stone s) {
	return (s == Player) ? Opponent : Player;
}