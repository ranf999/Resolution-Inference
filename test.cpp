#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "global.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#include "Resolution.h"

TEST_CASE("test equals", "[equals]")
{
	vector<string> var = { "x","y" };
	Predicate* P = new Predicate("P",var,true);
	Predicate* Q = new Predicate("P", var, true);
	vector<Predicate*> clause1;
	vector<Predicate*> clause2;
	clause1.push_back(P);
	clause2.push_back(Q);
	Clause* clauseA = new Clause(clause1);
	Clause* clauseB = new Clause(clause2);
	REQUIRE(equals(clauseA,clauseB)==true);
}
