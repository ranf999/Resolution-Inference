#ifndef _RESOLUTION_H_
#define _RESOLUTION_H 


#include "Clause.h"


class Resolution
{
private:
	//vector<Clause> KB;
public:
	bool doResolution(vector<Clause*> KB,Clause* query, chrono::system_clock::time_point start);
	bool hasEmpty(Clause* clauses);

	bool equals(Clause* A, Clause* B);
	vector<Clause*> disjunct(vector<Clause*> clausesA,vector<Clause*> clausesB);
	bool belongTo(vector<Clause*> newClauses, vector<Clause*> KB);
	Clause* resolve(Clause* A, Clause* B);
	vector<unordered_map<string, string> > substitution(Predicate* preA, Predicate* preB);
	void unification(vector<Predicate*>& preA, vector<Predicate*>& preB, vector<unordered_map<string, string> > replace);
	bool unify(Predicate* preA, Predicate* preB, vector<Predicate* >& preVectorA, vector<Predicate*>& preVectorB);
};




#endif
