#pragma once
#include <list>
#include "../Platform/api.h"

using namespace std;


typedef struct {
	int y;
	int x;
}Point;


typedef struct _MinmaxNode {
	char isMaxnode;				// 1: Min node , 0: Max node
	int weight;					// Node의 가중치 
	list<void*> child;			// 자식노드		
	_MinmaxNode* patent;		// 부모노드
	Point selec_Point[2];

}MinmaxNode;

typedef struct _MinmaxLeafNode {
	char LeafBoard[BOARD_SIZE][BOARD_SIZE];		// Node의 보드 상태
	char isMaxnode;								// 0: Min node , 1: Max node
	int weight;									// Node의 가중치 
	MinmaxNode* patent;						// 부모노드
}MinmaxLeafNode;
