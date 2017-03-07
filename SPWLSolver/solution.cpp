#include "solution.h"

Solution::Solution(Graph g, int s, int e) : graph(g), start(s), end(e)
{
	
}


Solution::~Solution()
{
}

void Solution::setTimeLimit(double t) {
	timeLimit = t;
}

void Solution::setMaxDrivingTime(double t) {
	maxDrivingTime = t;
}

void Solution::setOffTime(double t) {
	offTime = t;
}

bool Solution::solve() {
	GRBEnv *env = 0;
	GRBVar** x = 0;
	GRBVar* y = 0;
	GRBVar* t = 0;
	GRBVar* tm = 0;
	try {
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);

		x = new GRBVar*[graph.numberOfVectors];
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			x[i] = new GRBVar[graph.numberOfVectors];// model.addVars(graph.numberOfVectors);
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				ostringstream vname;
				vname << "x" << i << "." << j;
				x[i][j] = model.addVar(0.0, graph.availability[i][j], graph.oilCost[i][j], GRB_BINARY, vname.str());
			}
		}

		y = new GRBVar[graph.numberOfVectors];
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			ostringstream vname;
			vname << "y" << i;
			y[i] = model.addVar(0.0, 1.0, 0, GRB_BINARY, vname.str());
		}

		
		t = model.addVars(graph.numberOfVectors);
		tm = model.addVars(graph.numberOfVectors);
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			ostringstream vname;
			vname << "t" << i;
			t[i].set(GRB_DoubleAttr_UB, maxDrivingTime);
			t[i].set(GRB_DoubleAttr_Obj, 1);
			t[i].set(GRB_StringAttr_VarName, vname.str());
			vname << "a";
			tm[i].set(GRB_DoubleAttr_UB, maxDrivingTime);
			tm[i].set(GRB_DoubleAttr_Obj, 1);
			tm[i].set(GRB_StringAttr_VarName, vname.str());
		}

		GRBLinExpr objExpr = 0;
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				if (i == j) {
					continue;
				}
				objExpr += graph.oilCost[i][j] * x[i][j];
			}
		}
		for (int i = 0; i < graph.numberOfVectors; i++)
		{
			//objExpr += y[i];
		}
		// Set objective
		model.setObjective(objExpr, GRB_MINIMIZE);

		// Add constraint
		GRBLinExpr lhsExpr = 0;
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			lhsExpr = 0;
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				if (i == j) {
					continue;
				}
				lhsExpr += x[i][j] - x[j][i];
			}
			ostringstream cname;
			cname << "C" << i;
			if (i == start) {
				model.addConstr(lhsExpr, GRB_EQUAL, 1, cname.str());
			}
			else if (i == end) {
				model.addConstr(lhsExpr, GRB_EQUAL, -1, cname.str());
			}
			else {
				model.addConstr(lhsExpr, GRB_EQUAL, 0, cname.str());
			}
		}

		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			lhsExpr = 0;
			ostringstream cname;
			cname << "Cy" << i;
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				lhsExpr += x[j][i];
			}
			model.addConstr(lhsExpr, GRB_GREATER_EQUAL, y[i], cname.str());
		}

		// Add max succesive driving time constraint
		GRBQuadExpr expr = 0;
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			expr = 0;
			ostringstream cname;
			cname << "DrivingTime" << i;
			if (i == start) {
				model.addQConstr(t[i], GRB_EQUAL, 0, cname.str() + "-1");
				continue;
			}
			
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				if (graph.availability[i][j] == 1) {
					expr += (tm[j] + graph.timeCost[j][i]) * x[j][i];
				}	
			}
			model.addQConstr(expr, GRB_EQUAL, t[i], cname.str() + "-1");
			model.addQConstr((1 - y[i]) * t[i], GRB_EQUAL, tm[i], cname.str() + "-2");
		}

		// Add total time constraint
		lhsExpr = 0;
		for (int i = 0; i < graph.numberOfVectors; ++i)
		{
			for (int j = 0; j < graph.numberOfVectors; ++j)
			{
				if (i == j) {
					continue;
				}
				lhsExpr += x[i][j] * graph.timeCost[i][j];
			}
			lhsExpr += y[i] * offTime;
		}
		model.addConstr(lhsExpr, GRB_LESS_EQUAL, timeLimit, "TotalTime");

		// Optimize model
		model.optimize();

		int status = model.get(GRB_IntAttr_Status);
		if (status == GRB_UNBOUNDED)
		{
			cout << "The model cannot be solved "
				<< "because it is unbounded" << endl;
			return false;
		}
		if (status == GRB_OPTIMAL)
		{
			cout << "The optimal objective is " <<
				model.get(GRB_DoubleAttr_ObjVal) << endl;
			for (int i = 0; i < graph.numberOfVectors; i++)
			{
				for (int j = 0; j < graph.numberOfVectors; j++)
				{
					if (x[i][j].get(GRB_DoubleAttr_X) == 1) {
						cout << x[i][j].get(GRB_StringAttr_VarName) << " ";
					}
				}
			}
			cout << endl;
			for (int i = 0; i < graph.numberOfVectors; i++)
			{
				if (y[i].get(GRB_DoubleAttr_X) == 1) {
					cout << y[i].get(GRB_StringAttr_VarName) << " ";
				}
			}
			/*cout << endl;
			for (int i = 0; i < graph.numberOfVectors; i++)
			{
				cout << t[i].get(GRB_DoubleAttr_X) << " ";
			}
			cout << endl;
			for (int i = 0; i < graph.numberOfVectors; i++)
			{
				cout << tm[i].get(GRB_DoubleAttr_X) << " ";
			}*/
			return true;
		}
		if ((status != GRB_INF_OR_UNBD) && (status != GRB_INFEASIBLE))
		{
			cout << "Optimization was stopped with status " << status << endl;
			return false;
		}

	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...) {
		cout << "Exception during optimization" << endl;
	}
	for (int i = 0; i < graph.numberOfVectors; ++i) {
		delete[] x[i];
	}
	delete[] x;
	delete env;
	return false;
}