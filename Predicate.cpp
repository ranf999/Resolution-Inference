#include "Predicate.h"

Predicate(string name, vector<string> variable, bool positive)
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

bool isPositive()
{
	return this->positive;
}
