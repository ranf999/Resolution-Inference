#ifndef _CLAUSE_H_
#define _CLAUSE_H_

#include "global.h"
#include "Predicate.h"

class Clause
{
private:
	vector<Predicate*> clausevector;
public:
	Clause() {};
	Clause(vector<Predicate*> clausevector);
	void setClausevector(vector<Predicate*>& clausevector);
	vector<Predicate*> getClausevector();
	void addPredicate(Predicate* pre);
	void clear();
	void negate();

};

#endif
