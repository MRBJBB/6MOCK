#include "minmax_tree.h"

void MinmaxTree::init_Tree(int depth, char (*board)[BOARD_SIZE])
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
	Rec_Create(1, ptr_root);
	
}



int MinmaxTree::Search_Tree()
{
	Rec_Search_Max(1, ptr_root);
	
	return 0;
}


void MinmaxTree::Delete_Tree(int key) {




}

void MinmaxTree::Rec_Create(int depth, MinmaxNode* parent)
{
	int i, j, k, l;

	for (i = 0; i < BOARD_SIZE; i++) for (j = 0; j < BOARD_SIZE; j++) {
		if (game_Board[i][j] != 0) continue;

		if (depth % 2)	// ³» Â÷·Ê 
			game_Board[i][j] = 1;
		else
			game_Board[i][j] = 2;


		for (k = i; k < BOARD_SIZE; k++) for (l = j + 1; l < BOARD_SIZE; l++) {
			if (game_Board[k][l] != 0) continue;

			if (depth % 2)	// ³» Â÷·Ê 
				game_Board[k][l] = 1;
			else
				game_Board[k][l] = 2;


			if (depth != tree_Depth) {
				MinmaxNode* child = new MinmaxNode;

				child->patent = parent;
				child->isMaxnode = depth % 2;
				child->weight = -1;

				child->selec_Point[0].y = i; child->selec_Point[0].x = j;
				child->selec_Point[1].y = k; child->selec_Point[1].x = l;

				parent->child.push_back(child);
				Rec_Create(depth + 1,child);
			}
			else {

				MinmaxLeafNode* child = new MinmaxLeafNode;
				child->patent = parent;
				child->isMaxnode = depth % 2;
				parent->child.push_back(child);

				for (int y=0; y < BOARD_SIZE; y++)for (int x=0; x < BOARD_SIZE; x++) {
					child->LeafBoard[y][x] = game_Board[x][y];
				}
			}
			game_Board[k][l] = 0;
		}
		game_Board[i][j] = 0;
	}

}

void MinmaxTree::Rec_Search_Min(int depth, MinmaxNode * parent)
{

	list<void*>::iterator iter;
	for (iter = parent->child.begin(); iter != parent->child.end(); ++iter) {
		if (depth != tree_Depth) {
			if (((MinmaxNode*)(*iter))->weight == -1) {
				Rec_Search_Max(depth + 1, ((MinmaxNode*)(*iter)));
			}
			if (parent->weight < ((MinmaxNode*)(*iter))->weight) {
				parent->weight = ((MinmaxNode*)(*iter))->weight;
			}
		}
		else {
			if (parent->weight > ((MinmaxLeafNode*)(*iter))->weight)
				parent->weight = ((MinmaxLeafNode*)(*iter))->weight;
		}
	}


}

void MinmaxTree::Rec_Search_Max(int depth, MinmaxNode * parent)
{

	list<void*>::iterator iter;
	for (iter = parent->child.begin(); iter != parent->child.end(); ++iter) {
		if (((MinmaxNode*)(*iter))->weight == -1) {
			Rec_Search_Min(depth + 1, ((MinmaxNode*)(*iter)));
		}
		if (parent->weight < ((MinmaxNode*)(*iter))->weight) {
			parent->weight = ((MinmaxNode*)(*iter))->weight;
			if (depth == 1) {
				parent->selec_Point[0] = ((MinmaxNode*)(*iter))->selec_Point[0];
				parent->selec_Point[1] = ((MinmaxNode*)(*iter))->selec_Point[1];
			}
		}
		
		
	}

}



