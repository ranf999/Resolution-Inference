#ifndef _RESOLUTION_H_
#define _RESOLUTION_H 


#include "Clause.h"


class Resolution
{
private:
	//vector<Clause> KB;
public:
	bool doResolution(vector<Clause*> KB,Clause* query);
	bool hasEmpty(vector<Clause*>& clauses);

	bool equals(Clause* A, Clause* B);
	vector<Clause*> disjunct(vector<Clause*> clausesA,vector<Clause*> clausesB);
	bool belongTo(vector<Clause*> newClauses, vector<Clause*> KB);
	Clause* resolve(Clause* A, Clause* B);
	bool unify(Predicate* preA, Predicate* preB, vector<Predicate* >& preVectorA, vector<Predicate*>& preVectorB);
};




#endif
