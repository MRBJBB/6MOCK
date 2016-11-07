#include <cstdlib>
#include <time.h>
#include <algorithm>

#include "../Minmax/minmax_tree.h"
#include "../Platform/api.h"
#include "score.h"

using namespace std;

extern int PlayerMoveCnt;
extern float ScoreBoard[BOARD_SIZE][BOARD_SIZE];

void myturn(int cnt) {
	int x[2], y[2];
	
	

	if (PlayerMoveCnt == 0) {
		initPlayerBoard();
	}
	else {
		copyGameBoard(); 
		
	}

	if (cnt == 1) {
		float s = -1.0f;
		for (int i = 0; i<BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (s < ScoreBoard[i][j]) {
					s = ScoreBoard[i][j];
					x[0] = i, y[0] = j;
				}
			}
	}
	else {
		MinmaxTree tree;
		tree.init_Tree(2);
		tree.Create_Tree();
		tree.get_solution(x, y, cnt);
	}
	domymove(x, y, cnt);
}