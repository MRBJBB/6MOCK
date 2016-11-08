#include <thread>
#include "minmax_tree.h"
#include "../Player/score.h"

extern char PlayerBoard[BOARD_SIZE][BOARD_SIZE];
char MarkerBoard[BOARD_SIZE][BOARD_SIZE];

char ThreadBoard[NUMOFTHREAD][BOARD_SIZE][BOARD_SIZE];
Point Ptr_Thread[4][2];

void Devide_Thread(int nThread, MinmaxNode* parent);
void Opp_Evaluate(int depth, MinmaxNode* parent, float prev_score,int nThread);


void MinmaxTree::init_Tree(int depth)
{
	ptr_root = new MinmaxNode;
	tree_Depth = depth;

	memset(MarkerBoard, 0, sizeof(MarkerBoard));
	Ptr_Thread[0][0].x = 0; Ptr_Thread[0][0].y = 0;	Ptr_Thread[0][1].x = 10; Ptr_Thread[0][1].y = 10;
	Ptr_Thread[1][0].x = 0; Ptr_Thread[1][0].y = 10;	Ptr_Thread[1][1].x = 10; Ptr_Thread[1][1].y = 20;
	Ptr_Thread[2][0].x = 10; Ptr_Thread[2][0].y = 0;	Ptr_Thread[2][1].x = 20; Ptr_Thread[2][1].y = 10;
	Ptr_Thread[3][0].x = 10; Ptr_Thread[3][0].y = 10;	Ptr_Thread[3][1].x = 20; Ptr_Thread[3][1].y = 20;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			for (int k = 0; k < NUMOFTHREAD; k++)
				ThreadBoard[k][i][j] = PlayerBoard[i][j];

			if (PlayerBoard[i][j] == Player || PlayerBoard[i][j] == Opponent) {
				for (int x = -3; x < 4; x++) {
					for (int y = -3; y < 4; y++) {
						if (isInbound(i + x, j + y))
							MarkerBoard[i + x][j + y] = 1;
					}
				}
			}
		}
	}
}

void updateMarkerBoard() {

}




void MinmaxTree::Create_Tree()
{
	ptr_root->alpha = (float)NEGATIVE_INF;
	ptr_root->beta = (float)POSITIVE_INF;
	//Rec_Evaluate(1, ptr_root, 0);
	Stone stone = Player;
	
	thread trd1(&Devide_Thread, 0, ptr_root);
	thread trd2(&Devide_Thread, 1, ptr_root);
	thread trd3(&Devide_Thread, 2, ptr_root);
	thread trd4(&Devide_Thread, 3, ptr_root);

	trd1.join();
	trd2.join();
	trd3.join();
	trd4.join();
}

void Devide_Thread(int nThread, MinmaxNode* parent) {

	Stone stone = Player;
	

	for (int i = Ptr_Thread[nThread][0].x; i < Ptr_Thread[nThread][1].x; i++) for (int j = Ptr_Thread[nThread][0].y; j < Ptr_Thread[nThread][1].y; j++) {
		if (ThreadBoard[nThread][i][j] != Blank || MarkerBoard[i][j] == 0) continue;

		ThreadBoard[nThread][i][j] = stone;
		Point start, end;

		if (nThread == 0) {
			start.x = 0, start.y = 0;
		}
		else if( nThread == 3){
			start.x = i; start.y = j + 1;
		}
		else {
			start.x = Ptr_Thread[nThread][0].x; start.y = Ptr_Thread[nThread][0].y;
		}
		end.x = Ptr_Thread[3][1].x; end.y = Ptr_Thread[3][1].y;

		for (int k = start.x; k < end.x; k++) for (int l = start.y; l < end.y; l++) {
			if (ThreadBoard[nThread][k][l] != Blank || MarkerBoard[i][j] == 0) continue;
			if (nThread == 1 && k > Ptr_Thread[0][0].x  && k <= Ptr_Thread[0][1].x && l > Ptr_Thread[0][0].y  && l <= Ptr_Thread[0][1].y)
				continue;

			ThreadBoard[nThread][k][l] = stone;

			MinmaxNode child;

			child.alpha = parent->alpha;
			child.beta = parent->beta;
			child.parent = parent;

			child.point[0].x = i; child.point[0].y = j;
			child.point[1].x = k; child.point[1].y = l;

			int x[2] = { i, k };
			int y[2] = { j, l };

			float s = score(&(ThreadBoard[nThread]), x, y, 2, stone);

			s *= 1.0f;

			if (s >= 100.0f) { // wins
				parent->alpha = s;
				parent->point[0] = child.point[0];
				parent->point[1] = child.point[1];
				ThreadBoard[nThread][k][l] = Blank;
				ThreadBoard[nThread][i][j] = Blank;
				return;
			}

			Opp_Evaluate(2, &child, s,nThread);

			// alpha := max (alpha, alphabeta(child))
			if (parent->alpha < child.beta) {
				parent->alpha = child.beta;

				if (parent->alpha >= parent->beta) {
					ThreadBoard[nThread][k][l] = Blank;
					ThreadBoard[nThread][i][j] = Blank;
					return;
				}
				parent->point[0] = child.point[0];
				parent->point[1] = child.point[1];
			}
			ThreadBoard[nThread][k][l] = Blank;
		}
		ThreadBoard[nThread][i][j] = Blank;
	}
}

void Opp_Evaluate(int depth, MinmaxNode* parent, float prev_score, int nThread)
{
	Stone stone = Opponent;

	for (int i = 0; i < BOARD_SIZE; i++) for (int j = 0; j < BOARD_SIZE; j++) {
		if (ThreadBoard[nThread][i][j] != Blank || MarkerBoard[i][j] == 0) continue;
		ThreadBoard[nThread][i][j] = stone;

		for (int k = i; k < BOARD_SIZE; k++) for (int l = j + 1; l < BOARD_SIZE; l++) {
			if (ThreadBoard[nThread][k][l] != Blank || MarkerBoard[i][j] == 0) continue;
			ThreadBoard[nThread][k][l] = stone;

			MinmaxNode child;

			child.alpha = parent->alpha;
			child.beta = parent->beta;
			child.parent = parent;

			child.point[0].x = i; child.point[0].y = j;
			child.point[1].x = k; child.point[1].y = l;

			int x[2] = { i, k };
			int y[2] = { j, l };

			float s = score(&(ThreadBoard[nThread]), x, y, 2, stone);

			s *= (-1.0f);

			if (s <= -100.0f) { // loses
				parent->beta = s;
				ThreadBoard[nThread][k][l] = Blank;
				ThreadBoard[nThread][i][j] = Blank;
				return;
			}

			s += prev_score * 0.7f;
			child.beta = child.alpha = s;
			// beta := min (beta, alphabeta(child))
			if (parent->beta > child.alpha)
			{
				parent->beta = child.alpha;
				if (parent->alpha >= parent->beta) {
					ThreadBoard[nThread][k][l] = Blank;
					ThreadBoard[nThread][i][j] = Blank;
					return;
				}
			}

			ThreadBoard[nThread][k][l] = Blank;
		}
		ThreadBoard[nThread][i][j] = Blank;
	}

}


void MinmaxTree::get_solution(int x[], int y[], int cnt) {
	for (int i = 0; i < cnt; i++) {
		x[i] = ptr_root->point[i].x;
		y[i] = ptr_root->point[i].y;
	}
}