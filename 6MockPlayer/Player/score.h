#include "..\Platform\api.h"

#define MAX_SCORE 10000.0f
#define INITIAL_SCORE_FACTOR 100.0f
#define BLOCK_PENALTY_FACTOR 0.97f
#define BLOCK_PENALTY_DIST 3

enum Direction {
	Vertical = 0,
	Horizontal = 1,
	Diag45 = 2, // y = x line
	Diag135 = 3 // y = -x line
};


void initPlayerBoard(); 
int getBlocks(int bx[], int by[]);

float score(int x[], int y[], int cnt, Stone stone);

inline bool isInbound(int x, int y);
inline int maxNorm(int x1, int y1, int x2, int y2);
inline Stone getOpponent(Stone s);