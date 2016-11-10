#include "Predicate.h"

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

bool isPositive()
{
	return this->positive;
}
