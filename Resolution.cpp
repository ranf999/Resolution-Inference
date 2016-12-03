
#include "Resolution.h"

bool Resolution::doResolution(vector<Clause* > KB, Clause* query, chrono::system_clock::time_point start)
{  
	query->negate();
	KB.push_back(query);
	unordered_map<int, int> hasUsed;
	unordered_map<int, int>::iterator it;
	while(1)
 	{
		vector<Clause*> newClauses;
		int size = KB.size();
		vector<Clause*> resolvents;
		for(int i = 0; i < size; i++)
			for(int j = i + 1; j < size; j++)
 			{
				auto end = chrono::system_clock::now();
				auto duration = chrono::duration_cast<chrono::seconds>(end - start);
				//if (duration.count() >= 29) return false;
				it = hasUsed.find(i);
				if (it != hasUsed.end() && j <= hasUsed[i])
					continue;
				Clause* result = resolve(KB[i], KB[j]);
				if (hasUsed.find(i) == hasUsed.end())
					hasUsed.emplace(i, j);
				else
					hasUsed[i] = j;
				if (result->getClausevector().size() != 0)
				{
					resolvents.push_back(result);
					if (hasEmpty(result))
						return true;
				}
				
				newClauses = disjunct(newClauses, resolvents);
			}
		if (belongTo(newClauses, KB))
			return false;
		KB = disjunct(KB, newClauses);
	}
	return false;
} 

bool Resolution::hasEmpty(Clause* clause)
{
		vector<Predicate*> preVector = clause->getClausevector();
		if(preVector[0]->name.compare("empty")==0)
			return true;
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
			if (preVectorA[i]->positive != preVectorB[i]->positive)
				return false;
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
		   if(clause->equals(clauseInKB))
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

			if (clauseA->equals((*it))) {
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
	int sizeA = preVectorA.size();
	int sizeB = preVectorB.size();
	bool canResolve = false;
	vector<unordered_map<string, string> > replace;
	//judge whether can be resolve
	for (int i = 0; i < preVectorA.size(); i++)
	{
		bool haveResolution = false;
		Predicate* preA = new Predicate();
		preA = preVectorA[i];
		//if (preVectorA.size() == 0) break;
		//for (int j = 0; j < preVectorB.size(); j++)
		if(B->predicateMap.find(preA->name)!= B->predicateMap.end())
		{
			vector<int> preNo = B->predicateMap[preA->name];
			for (auto j : preNo)
			{
				Predicate* preB = new Predicate();
				preB = preVectorB[j];
				bool canUnify = false;
				if(((preA->isPositive() == true && preB->isPositive() == false) || (preA->isPositive() == false && preB->isPositive() == true)))
				{
					//have conflicts, judge whether they can be unified
					//if two predicates are all the same
					bool same = true;
					for (int m = 0; m < preA->variable.size(); m++)
					{
						if ((preA->variable[m][0] < 'A'||preA->variable[m][0]>'Z' )||preA->variable[m].compare(preB->variable[m]) != 0)
						{
							same = false;
							break;
						}

					}
/*					if (same == true)
					{
						bool haveUpper = false;
						for (int m = 0; m < preA->variable.size(); m++)
						{
							if (preA->variable[m][0] >= 'A' && preA->variable[m][0] <= 'Z')
								haveUpper = true;
						}
						if (haveUpper == false)
						{
							same = false;
						}
					}*/


					if (same == true)
					{
						haveResolution = true;
						canResolve = true;
						preVectorA.erase(preVectorA.begin() + i);
						i--;
						preVectorB.erase(preVectorB.begin() + j);
						//j--
						for (auto& no : preNo)
							no--;
						break;
					}
					replace = substitution(preA, preB);
					if (!(replace[0].empty() && replace[1].empty()))
						canUnify = true;
					if (canUnify == true)
					{
						haveResolution = true;
						canResolve = true;
						preVectorA.erase(preVectorA.begin() + i);
						i--;
						preVectorB.erase(preVectorB.begin() + j);
						//j--;
						for (auto& no : preNo)
							no--;
						break;
					}
				}
			}

		}
		if (haveResolution == true)
			break;
	}
	if (canResolve == true) {
		//???
		vector<Predicate*> newVectorA;
		vector<Predicate*> newVectorB;
		if (preVectorA.size() == 0 && preVectorB.size() == 0)
		{
			Clause* res = new Clause();
			vector<string> vars;
			Predicate* empty = new Predicate("empty",vars, true);
			newVectorA.push_back(empty);
			res->setClausevector(newVectorA);
			return res;
		}
		for (int i = 0; i < preVectorA.size(); i++)
		{
			Predicate* preInA = new Predicate(preVectorA[i]->name, preVectorA[i]->variable, preVectorA[i]->positive);
			newVectorA.push_back(preInA);
		}
		for (int i = 0; i < preVectorB.size(); i++)
		{
			Predicate* preInB = new Predicate(preVectorB[i]->name, preVectorB[i]->variable, preVectorB[i]->positive);
			newVectorB.push_back(preInB);
		}

		if(!replace.empty())
			unification(newVectorA,newVectorB,replace);
		for (auto preB : newVectorB)
			newVectorA.push_back(preB);
		Clause* res = new Clause();
		
		//unification
		//factoring
		//factoring(newVectorPredicate);
		
		res->setClausevector(newVectorA);
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

vector<unordered_map<string,string> > Resolution::substitution(Predicate* preA, Predicate* preB)
{   
	bool canUnify = false;
	vector<string> argA = preA->getVariable();
	vector<string> argB = preB->getVariable();
	bool hasVal = false;
	unordered_map<string,string> replaceA;
	unordered_map<string,string> replaceB;
	vector<unordered_map<string,string> > mapVector(2);
	//worth unify
	bool allConstants = true;
	for(int i = 0; i < argA.size(); i++)
	{
		if((argA[i][0] >= 'a' && argA[i][0] <= 'z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
			allConstants = false;
	}
	if(allConstants == false) return mapVector;
	
	
	
	for(int i = 0; i < argA.size();i++)
	{ 
		if((argA[i][0] >= 'A' && argA[i][0] <= 'Z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
			hasVal = true;
		if ((argB[i][0] >= 'A' && argB[i][0] <= 'Z') && (argA[i][0] >= 'a'&&argA[i][0] <= 'z'))
			hasVal = true;
	}

	

	if(hasVal==false) return mapVector;
	unordered_map<string,int> hasHash;
// subusititution generate
	for(int i = 0; i < argA.size();i++)
	{	
		if((argA[i][0] >= 'A' && argA[i][0] <= 'Z')&&(argB[i][0] >= 'A'&&argB[i][0] <= 'Z')&&(argA[i].compare(argB[i])!=0))
		{
				replaceA.clear();
				replaceB.clear();
				mapVector.push_back(replaceA);
				mapVector.push_back(replaceB);
				return mapVector;

		}
		if((argA[i][0] >= 'A' && argA[i][0] <= 'Z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
		{
			 unordered_map<string,string>::const_iterator got = replaceB.find(argB[i]);
			 if(got == replaceB.end())
			 {
			 	replaceB.emplace(argB[i],argA[i]);
			 }
			 else
			 {
				replaceA.clear();
				replaceB.clear();
				mapVector.push_back(replaceA);
				mapVector.push_back(replaceB);
				return mapVector;
			 }
			
		}
		else if ((argB[i][0] >= 'A' && argB[i][0] <= 'Z') && (argA[i][0] >= 'a'&&argA[i][0] <= 'z'))
		{ 
			unordered_map<string,string>::const_iterator got = replaceA.find(argA[i]);
			if(got == replaceA.end())
			{
				replaceA.emplace(argA[i],argB[i]);
			}
			else
		 	{
				replaceA.clear();
				replaceB.clear();
				mapVector.push_back(replaceA);
				mapVector.push_back(replaceB);
				return mapVector;

			}

		}
		else if((argA[i][0] >= 'a' && argA[i][0] <= 'z')&&(argB[i][0] >= 'a'&&argB[i][0] <= 'z'))
		{	
			unordered_map<string,string>::const_iterator gotA = replaceA.find(argA[i]);
			unordered_map<string,string>::const_iterator gotB = replaceB.find(argB[i]);
			if(gotA == replaceA.end())
			{
				replaceA.emplace(argA[i],argB[i]);
			}
			else if(gotB == replaceB.end())
			{
				replaceB.emplace(argB[i],argA[i]);
			}
		}
	}
	mapVector[0] = replaceA;
	mapVector[1] = replaceB;

	return mapVector;
}

void Resolution::unification(vector<Predicate*>& preA, vector<Predicate*>& preB, vector<unordered_map<string,string> > replace)
{
	unordered_map<string,string>::iterator it;
	for(auto pre : preA)
	{
		for(int i = 0; i < pre->variable.size(); i++)
		{
			it = replace[0].find(pre->variable[i]);
			if(it != replace[0].end())
			{
				pre->variable[i] = replace[0][pre->variable[i]];
			}
		}
	}

	for(auto pre : preB)
	{
		for(int i = 0; i < pre->variable.size(); i++)
		{
			it = replace[1].find(pre->variable[i]);
			if(it != replace[1].end())
			{
				pre->variable[i] = replace[1][pre->variable[i]];
			}
		}

	}


}


