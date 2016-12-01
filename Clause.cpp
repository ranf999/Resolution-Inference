#include "Clause.h"

Clause::Clause(vector<Predicate*> clausevector)
{
	this->clausevector = clausevector;
}

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

void Clause::addPredicate(Predicate* pre)
{
	this->clausevector.push_back(pre);
}

void Clause::clear()
{
	this->clausevector.clear();
}
