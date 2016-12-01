#include "Predicate.h"

Predicate::Predicate(string name, vector<string> variable, bool positive)
{
	this->name = name;
	this->variable = variable;
	this-> positive = positive;
}

string Predicate::getName()
{
	return this->name;
}

void Predicate::setName(string name)
{
	this->name = name;
}

vector<string> Predicate::getVariable()
{
	return this->variable;
}

void Predicate::setPositive(bool positive)
{
	this->positive = positive;
}

bool Predicate::isPositive()
{
	return this->positive;
}
