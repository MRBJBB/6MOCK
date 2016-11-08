#include "minmax_tree.h"
#include "../Player/score.h"

extern char PlayerBoard[BOARD_SIZE][BOARD_SIZE];
char MarkerBoard[BOARD_SIZE][BOARD_SIZE];


void MinmaxTree::init_Tree(int depth)
{
	ptr_root = new MinmaxNode;
	tree_Depth = depth;

	memset(MarkerBoard, 0, sizeof(MarkerBoard));

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
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
	ptr_root->alpha = NEGATIVE_INF;
	ptr_root->beta = POSITIVE_INF;
	Rec_Evaluate(1, ptr_root, 0);

}



// 트리의 생성과 탐색을 동시에 진행한다고 가정하고 함수 구현

void MinmaxTree::Rec_Create(int depth, MinmaxNode* parent)
{
	int i, j, k, l;

	for (i = 0; i < BOARD_SIZE; i++) for (j = 0; j < BOARD_SIZE; j++) {
		if (game_Board[i][j] != 0) continue;

		if (depth % 2)	// 내 차례 
			game_Board[i][j] = MYSTONE;
		else
			game_Board[i][j] = OPPSTONE;


		for (k = i; k < BOARD_SIZE; k++) for (l = j + 1; l < BOARD_SIZE; l++) {
			if (game_Board[k][l] != 0) continue;

			if (depth % 2)	// 내 차례 
				game_Board[k][l] = MYSTONE;
			else
				game_Board[k][l] = OPPSTONE;

			MinmaxNode* child = new MinmaxNode;

			child->alpha = parent->alpha;
			child->beta = parent->beta;

			child->parent = parent;
			//parent->child.push_back(child);
			child->point[0].y = i; child->point[0].x = j;
			child->point[1].y = k; child->point[1].x = l;

			if (depth != tree_Depth) {
				Rec_Create(depth + 1, child);
			}
			else {
				// game_Board를 parameter로 넘겨줘서 가중치 계산하는 함수 실행
				// child->val_alpha = Calc_Weight(game_Board, .... ); 
			}

			if (depth % 2) {
				if (parent->alpha < child->beta) {
					parent->alpha = child->beta;
					
					if (parent->alpha >= parent->beta) {
						game_Board[k][l] = 0;
						game_Board[i][j] = 0;
						return;
					}
					if (depth == 1)
					{
						parent->point[0] = child->point[0];
						parent->point[1] = child->point[1];
					}
				}
			}
			else {
				if (parent->beta > child->alpha)
				{
					parent->beta = child->alpha;
					if (parent->alpha >= parent->beta) {
						game_Board[k][l] = 0;
						game_Board[i][j] = 0;
						return;
					}
				}
			}

			game_Board[k][l] = 0;
		}
		game_Board[i][j] = 0;
	}

	//parent->child.clear();
}




void MinmaxTree::Rec_Evaluate(int depth, MinmaxNode* parent, float prev_score)
{
	Stone stone = depth % 2 ? Player : Opponent;

	for (int i = 0; i < BOARD_SIZE; i++) for (int j = 0; j < BOARD_SIZE; j++) {
		if (PlayerBoard[i][j] != Blank || MarkerBoard[i][j] == 0) continue;
		PlayerBoard[i][j] = stone;

		for (int k = i; k < BOARD_SIZE; k++) for (int l = j + 1; l < BOARD_SIZE; l++) {
			if (PlayerBoard[k][l] != Blank || MarkerBoard[i][j] == 0) continue;
			PlayerBoard[k][l] = stone;

			MinmaxNode* child = new MinmaxNode;

			child->alpha = parent->alpha;
			child->beta = parent->beta;

			child->parent = parent;
			//parent->child.push_back(child);
			child->point[0].x = i; child->point[0].y = j; 
			child->point[1].x = k; child->point[1].y = l;

			int x[2] = { i, k };
			int y[2] = { j, l };
			float s = score(x, y, 2, stone);
			s *= depth % 2 ? 1.0f : -1.0f;
			if (s >= 100.0f) { // wins
				if (depth == 1) {
					parent->point[0] = child->point[0];
					parent->point[1] = child->point[1];
					return;
				}
			}
			else if (s <= -100.0f) { // loses
				if (depth == 2) {
					parent->beta = POSITIVE_INF;
					parent->point[0] = child->point[0];
					parent->point[1] = child->point[1];
					return;
				}
			}

			s += prev_score * 0.7f;

			if (depth != tree_Depth) {
				Rec_Evaluate(depth + 1, child, s);
			}
			else {
				// game_Board를 parameter로 넘겨줘서 가중치 계산하는 함수 실행
				// child->val_alpha = Calc_Weight(game_Board, .... ); 
				child->beta = child->alpha = s;
			}

			if (depth % 2) {
				// alpha := max (alpha, alphabeta(child))
				if (parent->alpha < child->beta) {
					parent->alpha = child->beta;

					if (parent->alpha >= parent->beta) {
						PlayerBoard[k][l] = Blank;
						PlayerBoard[i][j] = Blank;
						return;
					}
					if (depth == 1)
					{
						parent->point[0] = child->point[0];
						parent->point[1] = child->point[1];
					}
				}
			}
			else {
				// beta := min (beta, alphabeta(child))
				if (parent->beta > child->alpha)
				{
					parent->beta = child->alpha;
					if (parent->alpha >= parent->beta) {
						PlayerBoard[k][l] = Blank;
						PlayerBoard[i][j] = Blank;
						return;
					}
				}
			}
			delete child;
			PlayerBoard[k][l] = Blank;
		}
		PlayerBoard[i][j] = Blank;
	}

	//parent->child.clear();
}


void MinmaxTree::get_solution(int x[], int y[], int cnt) {
	for (int i = 0; i < cnt; i++) {
		x[i] = ptr_root->point[i].x;
		y[i] = ptr_root->point[i].y;
	}
}