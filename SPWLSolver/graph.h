#pragma once

#include <vector>
using namespace std;
class Graph
{
public:
	vector<int> timeOff;
	vector<int> isOff;
	vector<vector<int>> isInPath;
	Graph(int v, int e);
	~Graph();

	class Vector
	{
	public:
		int identifier;
		bool isRest;
	};
public:
	vector<Vector> vec;
	int numberOfVectors;
	int numberOfEdges;
	vector<vector<int>> oilCost;
	vector<vector<int>> timeCost;
	vector<vector<int>> availability;

public:
	void init();
};

