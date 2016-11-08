#pragma once
#include "minmax_node.h"
#include "../Platform/api.h"

class MinmaxTree {

private:
	MinmaxNode* ptr_root;
	int tree_Depth;
public:

	void init_Tree(int depth);					// ���� ������ ���� �Ұ� �� ó������ ����ؾ� �ϹǷ� Ʈ�� �ʱ�ȭ
	void Create_Tree();		// Ʈ�� Ȯ�� �� Leaf ����� ����ġ ���
	void get_solution(int x[], int y[], int cnt);
};