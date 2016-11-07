#pragma once
#include "minmax_node.h"
#include "../Platform/api.h"

class MinmaxTree {

private:
	MinmaxNode* ptr_root;
	int tree_Depth;
	char game_Board[BOARD_SIZE][BOARD_SIZE];
public:

	void init_Tree(int depth);					// ���� ������ ���� �Ұ� �� ó������ ����ؾ� �ϹǷ� Ʈ�� �ʱ�ȭ
	void Create_Tree();		// Ʈ�� Ȯ�� �� Leaf ����� ����ġ ���

	void Rec_Create(int depth, MinmaxNode* parent);
	void Rec_Evaluate(int depth, MinmaxNode* parent, float prev_score);

	void get_solution(int x[], int y[], int cnt);
};