#pragma once
#include "minmax_node.h"


class MinmaxTree {

private:
	MinmaxNode* ptr_root;
	int tree_Depth;
	char game_Board[BOARD_SIZE][BOARD_SIZE];
public:

	void init_Tree(int depth, char(*board)[BOARD_SIZE]);					// ���� ������ ���� �Ұ� �� ó������ ����ؾ� �ϹǷ� Ʈ�� �ʱ�ȭ
	void Create_Tree();		// Ʈ�� Ȯ�� �� Leaf ����� ����ġ ���

	void Rec_Create(int depth, MinmaxNode* parent);

};