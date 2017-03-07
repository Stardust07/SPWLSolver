#pragma once
#include "graph.h"
#include "gurobi_c++.h"
#include <sstream>
class Solution
{
public:
	Graph graph;
	int start, end;
	int timeLimit;
	int maxDrivingTime;
public:
	Solution(Graph g, int s, int e);
	~Solution();
	bool solve();
	void setTimeLimit(int t);
	void setMaxDrivingTime(int t);

	GRBQuadExpr generateDrivingTime(int v, GRBVar** x, GRBVar* y);
};

