#include "Clause.h"

vector<Predicate*> Clause::getClausevector()
{
	return this->clausevector;
}

void Clause::setClausevector(vector<Predicate*>& clausevector)
{
	this->clausevector = clausevector;
}

void Clause::negate()
{
	for(auto pre:clausevector)
		pre->setPositive(false);
}


