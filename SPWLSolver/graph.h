#pragma once

#include <vector>
using namespace std;
class Graph
{
public:
	int numberOfVectors;
	int numberOfEdges;
	vector<vector<int>> available;

	class Vector
	{
	public:
		int identifier;
		bool isRest;

	private:

	};
public:
	vector<Vector> vec;

	Graph();
	~Graph();
};

