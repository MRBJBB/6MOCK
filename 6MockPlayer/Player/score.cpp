#include <assert.h>
#include <initializer_list>

#include "score.h"

using namespace std;

/* Read Only */
extern char GameBoard[BOARD_SIZE][BOARD_SIZE];

float ScoreBoard[BOARD_SIZE][BOARD_SIZE];
char ActiveArea[BOARD_SIZE][BOARD_SIZE];
char PlayerBoard[BOARD_SIZE][BOARD_SIZE];

int dirDeltaX[8] = {-1, -1, 0, 1, 1,  1,  0, -1}; // 왼쪽 최상단이 (0, 0)
int dirDeltaY[8] = { 0,  1, 1, 1, 0, -1, -1, -1};

void initPlayerBoard() {
	memcpy(PlayerBoard, GameBoard, sizeof(PlayerBoard));

	// 블록이 있는 위치
	int bcnt, bx[6], by[6];
	bcnt = getBlocks(bx, by);

	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			// 중심에 가까울수록 유리
			int dist = maxNorm(x, y, BOARD_CENTER, BOARD_CENTER);
			ScoreBoard[x][y] = (float)INITIAL_SCORE_FACTOR * (1.0f - (float)dist / (float)BOARD_SIZE);

			// 블록에 가까울수록 불리
			for (int i = 0; i < bcnt; i++) {
				dist = maxNorm(x, y, bx[i], by[i]);
				int penalty = max(BLOCK_PENALTY_DIST - dist, 0);
				ScoreBoard[x][y] *= (1.0f - (float)BLOCK_PENALTY_FACTOR * (float)penalty);
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

void copyGameBoard() {
	memcpy(PlayerBoard, GameBoard, sizeof(PlayerBoard));
}

// 해당 위치에 돌을 놓았을 때의 점수 (돌이 이미 보드에 놓여있어야 함)
// TODO: 점수 계산시 7목 조심하기
float score(int x[], int y[], int cnt, Stone stone) {
	float res = 0.0f;
	LineInfo lineInfo[2];
	float lineScore[2][4];
	memset((void *)&lineInfo, 0, sizeof(lineInfo));

	for (int i = 0; i < cnt; i++) {
		assert(PlayerBoard[x[i]][y[i]] == stone);
		//PlayerBoard[x[i]][y[i]] = stone;
	}

	for (int i = 0; i < cnt; i++) {
		getLineInfo(x[i], y[i], stone, Vertical, lineInfo[i]);
		getLineInfo(x[i], y[i], stone, Diag45, lineInfo[i]);
		getLineInfo(x[i], y[i], stone, Horizontal, lineInfo[i]);
		getLineInfo(x[i], y[i], stone, Diag135, lineInfo[i]);
		lineScore[i][Vertical] = getLineScore(lineInfo[i], Vertical);
		lineScore[i][Diag45] = getLineScore(lineInfo[i], Diag45);
		lineScore[i][Horizontal] = getLineScore(lineInfo[i], Horizontal);
		lineScore[i][Diag135] = getLineScore(lineInfo[i], Diag135);
	}

	for (int i = 0; i < cnt; i++) {
		res += ScoreBoard[x[i]][y[i]];
		for (int j = 0; j < 4; j++) {
			res += lineScore[i][j];
		}
	}

	//for (int i = 0; i < cnt; i++) 
	//	PlayerBoard[x[i]][y[i]] = Blank;

	return res;
}

float getLineScore(LineInfo &info, Line line)
{
	int len = info.len[line];
	float result = 0;
	if (len >= 6) // connect 6!
		return 100.0f;
	if (info.bound[line] != 0 && info.bound[line + 4] != 0)
		return (float)len * 0.01f;
	if (info.bound[line] != 0 && info.bound[line + 4] == 0) {
		return (float)(len * len) * 
			(0.1f + 0.02f * (float)info.free[line + 4]);
	}
	if (info.bound[line] == 0 && info.bound[line + 4] != 0) {
		return (float)(len * len) *
			(0.1f + 0.02f * (float)info.free[line]);
	}
	if (info.bound[line] == 0 && info.bound[line + 4] == 0) {
		return (float)(len * len) * 
			(0.5f + 0.04f * (float)info.free[line] + 0.04f * (float)info.free[line + 4]);
	}
	return result;
}


void getLineInfo(int x, int y, Stone stone, Line line, LineInfo& lineInfo) {
	lineInfo.len[line] = 1;
	getDirInfo(x, y, stone, (Direction)line, lineInfo);
	getDirInfo(x, y, stone, (Direction)(line + 4), lineInfo);
}

void getDirInfo(int x, int y, Stone stone, Direction dir, LineInfo& lineInfo) {
	int dx = dirDeltaX[dir], dy = dirDeltaY[dir];
	do { // Blank일때만 탈출
		x += dx, y += dy;
		if (!isInbound(x, y)) { // 보드 바깥
			lineInfo.bound[dir] = 1;
			return;
		}
		if (PlayerBoard[x][y] == stone) {
			lineInfo.len[dir % 4]++;
		}
		else if (PlayerBoard[x][y] != Blank) { // 다른 Stone 또는 Block
			lineInfo.bound[dir] = 1;
			return;
		}
		else {
			break;
		}
	} while (true);

	while (isInbound(x, y) && PlayerBoard[x][y] == Blank && lineInfo.free[dir] < 3) {
		x += dx, y += dy;
		lineInfo.free[dir]++;
	}
	/*
	while (isInbound(x, y) && PlayerBoard[x][y] == stone) {
		x += dx, y += dy;
		lineInfo.sequence[dir]++;
	}
	*/
}

Connection getConnectionState(int x1, int y1, int x2, int y2) {
	bool containBlank = false;
	Stone stone = (Stone)PlayerBoard[x1][y1];
	int startX, startY, endX, endY, dist, dx, dy;
	if (x1 == x2) {
		startX = endX = x1;
		startY = min(y1, y2), endY = max(y1, y2);
		dist = endY - startY;
		dx = 0, dy = 1;
	}
	else if (y1 == y2) {
		startY = endY = y1;
		startX = min(x1, x2), endX = max(x1, x2);
		dist = endX - startX;
		dx = 1, dy = 0;
	}
	else if (x1 + y1 == x2 + y2) {
		startX = min(x1, x2), startY = max(y1, y2);
		endX = max(x1, x2), endY = min(y1, y2);
		dist = endX - startX;
		dx = 1, dy = -1;
	}
	else if (abs(x1 - y1) == abs(x2 - y2)) {
		startX = min(x1, x2), startY = min(y1, y2);
		endX = max(x1, x2), endY = max(y1, y2);
		dist = endX - startX;
		dx = 1, dy = 1;
	}
	else {
		return Unrelated;
	}

	if (dist > 5) return Unrelated;
	for (; startX <= endX && startY <= endY; startX += dx, startY += dy) {
		Stone s = (Stone)PlayerBoard[startX][startY];
		if (s == Blank) containBlank = true;
		else if (s == stone) continue;
		else return Unrelated;
	}
	return containBlank ? Connectable : Connected;
}

inline bool isInbound(int x, int y) {
	return x < BOARD_SIZE && y < BOARD_SIZE && x >= 0 && y >= 0;
}
inline int maxNorm(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
}
inline Stone getOpponent(Stone s) {
	return (s == Player) ? Opponent : Player;
}