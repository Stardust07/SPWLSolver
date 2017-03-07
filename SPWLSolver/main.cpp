#include "gurobi_c++.h"
#include "solution.h"
#include <sstream>
using namespace std;

bool readInstance(string filePath) {
	return true;
}

int main(int   argc, char *argv[]) {
	int v = 10;
	int e = 6;
	Graph g(v, e);
	int edges[][5] = {
		0, 1, 1, 0, 1,
		1, 0, 0, 1, 1,
		1, 0, 0, 0, 1,
		0, 1, 0, 0, 1,
		1, 1, 1, 1, 0
	};
	int time[][5] = {
		0, 4, 8, 0, 1,
		4, 0, 0, 1, 2,
		8, 0, 0, 0, 2,
		0, 1, 0, 0, 1,
		1, 2, 2, 1, 0
	};
	int oil[][5] = {
		0, 1, 5, 0, 6,
		1, 0, 0, 2, 4,
		5, 0, 0, 0, 3,
		0, 2, 0, 0, 7,
		6, 4, 3, 7, 0
	};
	for (int i = 0; i < v; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (i == j) {
				g.availability[i][j] = g.availability[j][i] = 0;
			}
			else {
				//g.availability[i][j] = g.availability[j][i] = edges[i][j];
				g.availability[i][j] = g.availability[j][i] = rand() % 2;
			}
			//g.timeCost[i][j] = g.timeCost[j][i] = time[i][j];
			//g.oilCost[i][j] = g.oilCost[j][i] = oil[i][j];
			g.timeCost[i][j] = g.timeCost[j][i] = rand() % 10 + 1;
			g.oilCost[i][j] = g.oilCost[j][i] = rand() % 10 + 1;
		}
	}
	for (int i = 0; i < v; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (g.availability[i][j] == 1) {
				cout << i << " " << j << " time=" << g.timeCost[j][i] << " oil=" << g.oilCost[j][i] << endl;
			}
		}
	}
	cout << endl;

	Solution sln(g, 0, 9);
	sln.setTimeLimit(20);
	sln.setMaxDrivingTime(4);
	sln.setOffTime(4);
	sln.solve();
	char c;
	cin >> c;

	return 0;
}