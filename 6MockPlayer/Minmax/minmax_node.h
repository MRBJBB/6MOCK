#pragma once
#include <list>
#include "../Platform/api.h"

using namespace std;

#define MYSTONE 1
#define OPPSTONE 2
#define POSITIVE_INF 0X7FFFFFFF
#define NEGATIVE_INF -0X7FFFFFFF

typedef struct {
	int x;
	int y;
}Point;


typedef struct _MinmaxNode {
	char isMaxnode;				// 1: Min node , 0: Max node
	float alpha;
	float beta;
	//list<_MinmaxNode*> child;			// �ڽĳ��		
	_MinmaxNode* parent;		// �θ���
	Point point[2];

}MinmaxNode;