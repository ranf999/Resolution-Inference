#include "Clause.h"

vector<Predicate> Clause::getClauseList()
{
	return this->clauseList;
}

void Clause::setClauseList(vector<Predicate> clauseList)
{
	this->clauseList = clauseList;
}
