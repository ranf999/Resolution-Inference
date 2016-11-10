#ifndef _CLAUSE_H_
#define _CLAUSE_H_

#include "global.h"
#include "Predicate.h"

class Clause
{
private:
	vector<Predicate> clauseList;
public:
	void setClauseList(vector<Predicate> clauseList);
	vector<Predicate> getClauseList;
}

#endif
