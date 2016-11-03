#pragma once
#include "minmax_node.h"


class MinmaxTree {

private:
	MinmaxNode* ptr_root;
	int tree_Depth;
	char game_Board[BOARD_SIZE][BOARD_SIZE];
public:

	void init_Tree(int depth, char(*board)[BOARD_SIZE]);					// 만약 데이터 저장 불가 시 처음부터 계산해야 하므로 트리 초기화
	void Create_Tree();		// 트리 확장 및 Leaf 노드의 가중치 계산

	void Rec_Create(int depth, MinmaxNode* parent);

};