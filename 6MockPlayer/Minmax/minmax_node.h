#pragma once
#include <vector>
#include <algorithm>
#include "../Platform/api.h"

using namespace std;

#define MYSTONE 1
#define OPPSTONE 2
#define POSITIVE_INF 0X7FFFFFFF
#define NEGATIVE_INF -0X7FFFFFFF
#define NUMOFTHREAD 4

typedef struct {
	int x;
	int y;
}Point;


typedef struct _Candidate{
	int score;
	int x;
	int y;
	bool operator<(_Candidate other) const
	{
		return score > other.score;
	}
}Candidate;

typedef struct _Solution {
	float score;
	Point points[2];
	bool operator<(_Solution other) const
	{
		return score > other.score;
	}
}Solution;

typedef struct {
	int len;
	Point start;
	Point end;
}Segment;

typedef struct _MinmaxNode{
	char isMaxnode;				// 1: Min node , 0: Max node
	float alpha;
	float beta;
	_MinmaxNode* parent;		// 부모노드
	Point point[2];
	
}MinmaxNode;