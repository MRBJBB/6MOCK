#include <cstdlib>
#include <time.h>

#include "../Platform/api.h"

using namespace std;


void myturn(int cnt) {
	int x[2], y[2];
	
	/*
	srand(time(NULL));
	for (int i = 0; i < cnt; i++) {
		while (true) {
			int xx = rand() % BOARD_SIZE;
			int yy = rand() % BOARD_SIZE;
			if (isfree(xx, yy)) {
				x[i] = xx;
				y[i] = yy;
				break;
			}
		}
	}
	*/

	if (cnt == 1) {
		
	}
	else {

	}

	domymove(x, y, cnt);
}