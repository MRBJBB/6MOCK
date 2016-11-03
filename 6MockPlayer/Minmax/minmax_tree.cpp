#include "minmax_tree.h"

void MinmaxTree::init_Tree(int depth, char(*board)[BOARD_SIZE])
{
	ptr_root = new MinmaxNode;
	tree_Depth = depth;


	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			game_Board[i][j] = board[i][j];
		}
	}
}

void MinmaxTree::Create_Tree()
{
	ptr_root->val_alpha = POSITIVE_INF;
	ptr_root->val_beta = NEGATIVE_INF;
	Rec_Create(1, ptr_root);

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

			child->val_alpha = parent->val_alpha;
			child->val_beta = parent->val_beta;

			child->patent = parent;
			parent->child.push_back(child);
			child->selec_Point[0].y = i; child->selec_Point[0].x = j;
			child->selec_Point[1].y = k; child->selec_Point[1].x = l;

			if (depth != tree_Depth) {
				Rec_Create(depth + 1, child);
			}
			else {
				// game_Board를 parameter로 넘겨줘서 가중치 계산하는 함수 실행
				// child->val_alpha = Calc_Weight(game_Board, .... ); 
			}

			if (depth % 2) {
				if (parent->val_alpha < child->val_beta) {
					parent->val_alpha = child->val_beta;
					
					if (parent->val_alpha >= parent->val_beta) {
						game_Board[k][l] = 0;
						game_Board[i][j] = 0;
						return;
					}
					if (depth == 1)
					{
						parent->selec_Point[0] = child->selec_Point[0];
						parent->selec_Point[1] = child->selec_Point[1];
					}
				}
			}
			else {
				if (parent->val_beta > child->val_alpha)
				{
					parent->val_beta = child->val_alpha;
					if (parent->val_alpha >= parent->val_beta) {
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

	parent->child.clear();
}



