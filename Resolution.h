#ifndef _RESOLUTION_H_
#define _RESOLUTION_H 

#include "global.h"
#include "Clause.h"

bool preCmp(Predicate* a, Predicate* b)
{
	if(a->getName().compare(b->getName()) == 0)
	{
		vector<string> argsA = a->getVariable();
		vector<string> argsB = b->getVariable();
		for(int i = 0; i < argsA.size(); i++)
		{
			if(argsA[i].compare(argsB[i]) < 0)
				return true;
			else if(argsA[i].compare(argsB[i]) > 0)
				return false;
		}
	}
	else if(a->getName().compare(b->getName()) < 0)
		return true;
	else
		return false;
	return false;
}

class Resolution
{
private:
	//vector<Clause> KB;
public:
	bool doResolution(vector<Clause*> KB,Clause* query);
	bool hasEmpty(vector<Clause*>& clauses);

	bool equals(Clause* A, Clause* B);
	vector<Clause*> disjunct(vector<Clause*> clausesA,vector<Clause*> clausesB);
};




#endif
