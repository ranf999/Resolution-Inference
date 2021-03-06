//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"
#include "global.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#include "Resolution.h"

int main()
{
	vector<string> var = { "x","y" };
	vector<string> var2 = { "X","Y" };
	Predicate* P = new Predicate("P",var,true);
	Predicate* Q = new Predicate("P", var2, true);
	vector<Predicate*> clause1;
	vector<Predicate*> clause2;
	clause1.push_back(P);
	clause2.push_back(Q);
	Clause* clauseA = new Clause(clause1);
	Clause* query = new Clause(clause2);
	vector<Clause*> KB;
	KB.push_back(clauseA);
	Resolution resolution;
	bool e = resolution.doResolution(KB,query);
	cout << e << endl;
	system("PAUSE");
	return 0;
}
