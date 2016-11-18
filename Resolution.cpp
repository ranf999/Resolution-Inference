#include "Resolution.h"

bool Resolution::doResolution(vector<Clause*> KB, Clause*& query)
{ 
	KB.push_back(query->negate());
	vector<Clause*> newClauses;
	while(1)
	{
		int size = KB.size();
		vector<Clause*> resolvents;
		for(int i = 0; i < size; i++)
			for(int j = i + 1; j < size; j++)
			{
				resolvents.push_back(resolve(KB[i],KB[j]));
				if(hasEmpty(resolvents))
					return true;
				newClauses = disjunct(newClauses, resolvents);
				if(belongTo(newClauses,KB))
					return false;
				KB = disjunct(newClauses,KB);
			}
	}
	return false;
}

bool Resolution::hasEmpty(vector<Clause*>& clauses)
{
	for(auto clause:clauses)
	{
		if(clause == NULL)
			return true;
	}
	return false;
}



bool Resolution::equals(Clause* A, Clause* B)
{
	vector<Predicate*> preVectorA = A->getClausevector();
	vector<Predicate*> preVectorB = B->getClausevector();
	if(preVectorA.size() != preVectorB.size())
		return false;
	else
	{
		sort(preVectorA.begin(), preVectorA.end(), preCmp);
		sort(preVectorB.begin(), preVectorB.end(), preCmp);
		//unordered_map<string,int> A,B;
		int n = preVectorA.size();
		for(int i = 0; i < n; i++)
		{
			if(preVectorA[i]->getName().compare(preVectorB[i]->getName())==0)
			{
				int varSize = preVectorA[i]->getVariable().size();
				for(int j = 0; j < varSize; j++)
				{
					if(preVectorA[i]->getVariable()[j].compare(preVectorB[i]->getVariable()[j])!=0)
					return false;	
				}
			}
			else
				return false;
		}
		return true;
	}
}

vector<Clause*> Resolution::disjunct(vector<Clause*> clausesA,vector<Clause*> clausesB)
{
	vector<Clause*> mixed;
	for(auto clauseA:clausesA)
	{
		mixed.push_back(clauseA);
		for(auto it = clausesB.begin(); it != clausesB.end(); it++)
		{ 
			if(equals(clauseA,(*it))
			{
				it = clausesB.erase(it);
			}
		}
	}
	for(auto clauseB:clausesB)
		mixed.push_back(clauseB);
	return mixed;
}
agasggdagasga
gagagagsgsggaggg
sfagagga



