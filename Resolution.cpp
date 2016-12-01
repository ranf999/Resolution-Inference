
#include "Resolution.h"

bool Resolution::doResolution(vector<Clause* > KB, Clause* query)
{  
	query->negate();
	KB.push_back(query);
	vector<Clause*> newClauses;
	while(1)
 	{
		int size = KB.size();
		vector<Clause*> resolvents;
		for(int i = 0; i < size; i++)
			for(int j = i + 1; j < size; j++)
 			{
				Clause* result = resolve(KB[i], KB[j]);
				if(result->getClausevector().size()!=0)
					resolvents.push_back(result);
				if(hasEmpty(resolvents))
					return true;		
				newClauses = disjunct(newClauses, resolvents);
			}
		if (belongTo(newClauses, KB))
			return false;
		KB = disjunct(KB, newClauses);
	}
	return false;
} 

bool Resolution::hasEmpty(vector<Clause*>& clauses)
{
	for(auto clause:clauses)
	{
		vector<Predicate*> preVector = clause->getClausevector();
		if(preVector.size()==0)
			return true;
	}
	return false;
}

bool predicateCmp(Predicate* a, Predicate* b)
{
	if (a->getName().compare(b->getName()) == 0)
	{
		vector<string> argsA = a->getVariable();
		vector<string> argsB = b->getVariable();
		for (int i = 0; i < argsA.size(); i++)
		{
			if (argsA[i].compare(argsB[i]) < 0)
				return true;
			else if (argsA[i].compare(argsB[i]) > 0)
				return false;
		}
	}
	else if (a->getName().compare(b->getName()) < 0)
		return true;
	else
		return false;
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
		sort(preVectorA.begin(), preVectorA.end(), predicateCmp);
		sort(preVectorB.begin(), preVectorB.end(), predicateCmp);
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

bool Resolution::belongTo(vector<Clause*> newClauses, vector<Clause*> KB)
{
   for(auto clause : newClauses)
   { 
	   bool find = false;
	   for(auto clauseInKB : KB)
	   {
		   if(equals(clause, clauseInKB))
				find = true;
	   }
       if(find == false)
		   return false;
   }
   return true;
}

vector<Clause* > Resolution::disjunct(vector<Clause* > clausesA, vector<Clause* > clausesB)
{ 
	vector<Clause* > mixed;
	for(auto clauseA : clausesA)
	{
		mixed.push_back(clauseA);
		bool isbegin = false;
		if (clausesB.size() == 0) break;
		for (auto it = clausesB.begin(); it != clausesB.end(); it++) {
			if (isbegin == true)
			{
				it--;
				isbegin = false;
			}
			if (equals(clauseA, (*it))) {
				it = clausesB.erase(it);
				if (it == clausesB.begin())
					isbegin = true;
				else
					it--;
			}
			if (clausesB.size() == 0) break;
		}
	}
	if (clausesB.size() != 0)
		for(auto clauseB : clausesB)
		{
			mixed.push_back(clauseB);
		}
	return mixed;
}

Clause* Resolution::resolve(Clause* A, Clause* B)
{
	vector<Predicate*> preVectorA = A->getClausevector();
	vector<Predicate*> preVectorB = B->getClausevector();
	sort(preVectorA.begin(), preVectorA.end(), predicateCmp);
	sort(preVectorB.begin(), preVectorB.end(), predicateCmp);
	A->setClausevector(preVectorA);
	B->setClausevector(preVectorB);
	int sizeA = preVectorA.size();
	int sizeB = preVectorB.size();
	int n = sizeA < sizeB ? sizeA : sizeB;//????
	bool canResolve = false;
	//judge whether can be resolve
	for (int i = 0; i < preVectorA.size(); i++)
	{
		//if (preVectorA.size() == 0) break;
		for (int j = 0; j < preVectorB.size(); j++)
		{
			Predicate* preA = new Predicate();
			Predicate* preB = new Predicate();
			preA = preVectorA[i];
			preB = preVectorB[j];
			if (preA->getName().compare(preB->getName()) == 0
				&& ((preA->isPositive() == true && preB->isPositive() == false) || (preA->isPositive() == false && preB->isPositive() == true)))
			{
				//have conflicts, judge whether they can be unified
				//if two predicates are all the same
				bool same = true;
				for(int m = 0; m < preA->variable.size(); m++)
				{
					if(preA->variable[m].compare(preB->variable[m])!=0)
						same = false;
				}
			
				if(same == true)
				{
					canResolve = true;
					preVectorA.erase(preVectorA.begin() + i);
					i--;
					preVectorB.erase(preVectorB.begin() + j);
					j--;
					break;
				}
				unordered_map<string,string> replace = substitution(preA, preB);
				if(replace.size() > 0)
					canUnify = true;
				if (canUnify == true)
				{
					canResolve = true;
					preVectorA.erase(preVectorA.begin() + i);
					i--;
					preVectorB.erase(preVectorB.begin() + j);
					j--;
					break;
				}
			}
		}
	}
	if (canResolve == true) {
		vector<Predicate*> newVectorPredicate;
		for (auto preB : preVectorB)
			preVectorA.push_back(preB);
		Clause* res = new Clause();
		for(int i = 0; i < preVectorA.size(); i++)
		{
			Predicate* preInA = new Predicate(preVectorA->name, preVectorA->variable, preVectorA->positive);
			newVectorPredicate.push_back(preInA);
		}
		//unification
		//
		//
		res->setClausevector(newVectorPredicate);
		return res;
	}
		Clause* res = new Clause();
		return res;
	
}
	

bool Resolution::unify(Predicate* preA, Predicate* preB, vector<Predicate* >& preVectorA, vector<Predicate*>& preVectorB)
{ 
	bool canUnify = false;
	vector<string> argA = preA->getVariable();
	vector<string> argB = preB->getVariable();
	bool hasVal = false;
	for(int i = 0; i < argA.size();i++)
	{
		if((argA[i][0] >= 'A' && argA[i][0] <= 'Z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
			hasVal = true;
		if ((argB[i][0] >= 'A' && argB[i][0] <= 'Z') && (argA[i][0] >= 'a'&&argA[i][0] <= 'z'))
			hasVal = true;
	}
	
	if(hasVal==false) return false;
	for(int i = 0; i < argA.size(); i++)
	{
		if(argA[i][0]>='a' && argA[i][0]<='z')
		{
			canUnify=true;
			for(int m = 0; m < preVectorA.size(); m++)
			{
				vector<string> args = preVectorA[i]->getVariable();
				for(int n = 0; n < args.size(); n++)
				{
					if(preVectorA[m]->getVariable()[n].compare(argA[i]) == 0)
						preVectorB[m]->variable[n] = argB[i];
				}
			}
		}
		else if(argB[i][0]>='a' && argB[i][0]<='z')
		{
			canUnify = true;
			for(int m = 0; m < preVectorB.size(); m++)
			{
				vector<string> args = preVectorB[m]->getVariable();
				for(int n = 0; n < args.size(); n++)
				{
					if ((preVectorB[m]->getVariable())[n].compare(argB[i]) == 0)
					{
						preVectorB[m]->variable[n] = argA[i];
					}
				}
			}

		}
	}
	return canUnify;
}

unordered_map<string,string> Resolution::substitution(Predicate* preA, Predicate* preB)
{
	bool canUnify = false;
	vector<string> argA = preA->getVariable();
	vector<string> argB = preB->getVariable();
	bool hasVal = false;
	unordered_map<string,string> replace;
	for(int i = 0; i < argA.size();i++)
	{
		if((argA[i][0] >= 'A' && argA[i][0] <= 'Z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
		{
			
		}
		else if ((argB[i][0] >= 'A' && argB[i][0] <= 'Z') && (argA[i][0] >= 'a'&&argA[i][0] <= 'z'))
		{

		}
	}
	

}



