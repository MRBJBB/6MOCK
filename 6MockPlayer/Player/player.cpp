#include <cstdlib>
#include <time.h>

#include "../Platform/api.h"
#include "score.h"

using namespace std;

extern int PlayerMoveCnt;

void myturn(int cnt) {
	int x[2], y[2];
	
	if (PlayerMoveCnt == 0) {
		initPlayerBoard();
	}
	else {
		copyGameBoard();
	}

	domymove(x, y, cnt);
}