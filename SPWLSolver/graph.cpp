#include "Graph.h"


Graph::Graph(int v, int e) :numberOfVectors(v), numberOfEdges(e)
{
	availability = vector<vector<int>>(numberOfVectors, vector<int>(numberOfVectors, 0));
	oilCost = vector<vector<int>>(numberOfVectors, vector<int>(numberOfVectors, INT_MAX));
	timeCost = vector<vector<int>>(numberOfVectors, vector<int>(numberOfVectors, INT_MAX));
	init();
}


Graph::~Graph()
{
}

void Graph::init() {
	isInPath = vector<vector<int>>(numberOfVectors, vector<int>(numberOfVectors, 0));
	isOff = vector<int>(numberOfVectors, 1);
}