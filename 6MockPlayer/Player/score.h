#include "..\Platform\api.h"

#define MAX_SCORE 10000.0f
#define INITIAL_SCORE_FACTOR 0.1f
#define BLOCK_PENALTY_FACTOR 0.3f
#define BLOCK_PENALTY_DIST 3

enum Line {
	Vertical = 0, // N
	Diag45, // NE
	Horizontal, // E
	Diag135 // SE
};

enum Direction {
	N = 0, NE, E, SE, 
	S, SW, W, NW
};

typedef struct {
	int len[4];
	int bound[8]; // ���������� 0 �ƴϸ� 1
	int free[8]; // �̾� ���� �� �ִ� ��ĭ�� ��
	int sequence[8]; // unbound �϶� �̾ �����ִ� ���� ��
} LineInfo;

enum Connection {
	Connected, // ���� ��
	Connectable, // ���� ���ɼ� ���� (�Ÿ� 6 ����)
	Unrelated // ���� ���ɼ� ����, ���θ����ִ� ��쵵 ����
};

void initPlayerBoard(); 
int getBlocks(int bx[], int by[]);
void copyGameBoard();

float score(int x[], int y[], int cnt, Stone stone);
float getLineScore(LineInfo &info, Line line);
void getLineInfo(int x, int y, Stone stone, Line line, LineInfo& lineInfo);
void getDirInfo(int x, int y, Stone stone, Direction dir, LineInfo& lineInfo);
Connection getConnectionState(int x1, int y1, int x2, int y2);

inline bool isInbound(int x, int y);
inline int maxNorm(int x1, int y1, int x2, int y2);
inline Stone getOpponent(Stone s);