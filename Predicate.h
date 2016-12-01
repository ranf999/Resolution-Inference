#ifndef _PREDICATE_H_
#define _PREDICATE_H_

#include "global.h"

class Predicate
{
public:
	string name;
	vector<string> variable;
	bool positive;
public:
	Predicate() {};
	Predicate(string name, vector<string> variable, bool positive);
	string getName();
	void setName(string name);
	vector<string> getVariable();
	//void setvariable(vector<string> variable);
	bool isPositive();
	void setPositive(bool positive);
};
#endif
