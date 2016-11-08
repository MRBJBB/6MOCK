#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include "minmax_tree.h"
#include "../Player/score.h"

extern int OpponentMoveCnt;
extern Point OpponentMovement[2];
extern int dirDeltaX[8];
extern int dirDeltaY[8];

extern char PlayerBoard[BOARD_SIZE][BOARD_SIZE];
char MarkerBoard[BOARD_SIZE][BOARD_SIZE];

char ThreadBoard[NUMOFTHREAD][BOARD_SIZE][BOARD_SIZE];


void Devide_Thread(int nThread, MinmaxNode* parent);
void Opp_Evaluate(int depth, MinmaxNode* parent, float prev_score,int nThread);

vector<Candidate> candidates = {};


/* MS Time Check*/
volatile bool timeout = false;

static DWORD CALLBACK timeCheck(LPVOID p)
{
	
	SleepEx(6500, FALSE);
	timeout = true;
	return 0;
}

//thread timer(&timeCheck, 6500);
// timer.join() 하면 됨.

void MinmaxTree::init_Tree(int depth)
{
	tree_Depth = depth;
	candidates.clear();
	
	memset(MarkerBoard, 0, sizeof(MarkerBoard));
	
	for (int k = 0; k < NUMOFTHREAD; k++) {
		ptr_root[k] = new MinmaxNode;
		memcpy(ThreadBoard[k], PlayerBoard, sizeof(PlayerBoard));
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {

			if (PlayerBoard[i][j] == Player || PlayerBoard[i][j] == Opponent) {
				for (int x = -3; x < 4; x++) {
					for (int y = -3; y < 4; y++) {
						if (isInbound(i + x, j + y))
							MarkerBoard[i + x][j + y] += 4 - max(abs(x), abs(y));
					}
				}
			}
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (MarkerBoard[i][j] > 0 && PlayerBoard[i][j] == Blank) {
				Candidate c = { MarkerBoard[i][j], i, j };
				candidates.push_back(c);
			}
		}
	}
	sort(candidates.begin(), candidates.end());
}

// PlayerBoard only
inline bool isStone(int x, int y, Stone stone) {
	return isInbound(x, y) && PlayerBoard[x][y] == stone;
}

void MinmaxTree::Create_Tree()
{
	for (int k = 0; k < NUMOFTHREAD; k++) {
		ptr_root[k]->alpha = (float)NEGATIVE_INF;
		ptr_root[k]->beta = (float)POSITIVE_INF;
	}
	//Rec_Evaluate(1, ptr_root, 0);
	Stone stone = Player;
	
	//상대방이 다음 턴에 육목을 놓을 수 있는지 체크
	//if (checkThreat())
		//cout << "threat!" << endl;

	timeout = false;
	HANDLE timer = CreateThread(NULL, 0, &timeCheck, 0, 0, 0);
	thread trd1(&Devide_Thread, 0, ptr_root[0]);
	thread trd2(&Devide_Thread, 1, ptr_root[1]);
	thread trd3(&Devide_Thread, 2, ptr_root[2]);
	thread trd4(&Devide_Thread, 3, ptr_root[3]);
	

	trd1.join();
	trd2.join();
	trd3.join();
	trd4.join();
	CloseHandle(timer);	
}

void Devide_Thread(int nThread, MinmaxNode* parent) {

	Stone stone = Player;

	for (int t1 = nThread; t1 < candidates.size(); t1 += NUMOFTHREAD) {
		Candidate c1 = candidates.at(t1);
		int i = c1.x, j = c1.y;

		if (ThreadBoard[nThread][i][j] != Blank) continue;

		ThreadBoard[nThread][i][j] = stone;

		for (int t2 = t1 + 1; t2 < candidates.size(); t2++) {

			if (timeout) return;

			Candidate c2 = candidates.at(t2);
			int k = c2.x, l = c2.y;

			if (ThreadBoard[nThread][k][l] != Blank) continue;

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

			Opp_Evaluate(2, &child, s, nThread);

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

	for (int t1 = 0; t1 < candidates.size(); t1++) {
		Candidate c1 = candidates.at(t1);
		int i = c1.x, j = c1.y;

		if (ThreadBoard[nThread][i][j] != Blank) continue;
		ThreadBoard[nThread][i][j] = stone;

		for (int t2 = t1 + 1; t2 < candidates.size(); t2++) {

			if (timeout) return;

			Candidate c2 = candidates.at(t2);
			int k = c2.x, l = c2.y;
			if (ThreadBoard[nThread][k][l] != Blank) continue;
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
	int maxidx = 0; float max_score = NEGATIVE_INF;
	for (int i = 0; i < NUMOFTHREAD; i++) {
		if (max_score < ptr_root[i]->alpha) {
			max_score = ptr_root[i]->alpha;
			maxidx = i;
		}
		
	}
	for (int i = 0; i < cnt; i++) {
		x[i] = ptr_root[maxidx]->point[i].x;
		y[i] = ptr_root[maxidx]->point[i].y;
	}
	for (int i = 0; i < NUMOFTHREAD; i++)
		delete ptr_root[i];
}