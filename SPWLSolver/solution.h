#pragma once
#include "graph.h"
#include "gurobi_c++.h"
#include <sstream>
class Solution
{
public:
	Graph graph;
	int start, end;
	double timeLimit;
	double maxDrivingTime;
	double offTime;
public:
	Solution(Graph g, int s, int e);
	~Solution();
	bool solve();
	void setTimeLimit(double t);
	void setMaxDrivingTime(double t);
	void setOffTime(double t);
	GRBQuadExpr generateDrivingTime(int v, GRBVar** x, GRBVar* y);
};

