#pragma once
#include <list>
#include "../Platform/api.h"

using namespace std;

#define MYSTONE 1
#define OPPSTONE 2
#define POSITIVE_INF 0X7FFFFFFF
#define NEGATIVE_INF 0X80000001

typedef struct {
	int y;
	int x;
}Point;


typedef struct _MinmaxNode {
	char isMaxnode;				// 1: Min node , 0: Max node
	int val_alpha;
	int val_beta;
	list<void*> child;			// 자식노드		
	_MinmaxNode* patent;		// 부모노드
	Point selec_Point[2];

}MinmaxNode;