#include "Clause.h"

Clause::Clause(vector<Predicate*> clausevector)
{
	this->clausevector = clausevector;
	for (int i = 0; i < clausevector.size(); i++)
	{
		if (predicateMap.find(clausevector[i]->name) == predicateMap.end())
		{
			vector<int> position;
			position.push_back(i);
			predicateMap.emplace(clausevector[i]->name, position);
		}
		else
		{
			predicateMap[clausevector[i]->name].push_back(i);
		}
	}
}

vector<Predicate*> Clause::getClausevector()
{
	return this->clausevector;
}

void Clause::setClausevector(vector<Predicate*>& clausevector)
{
	this->clausevector = clausevector;
	for (int i = 0; i < clausevector.size(); i++)
	{
		if (predicateMap.find(clausevector[i]->name) == predicateMap.end())
		{
			vector<int> position;
			position.push_back(i);
			predicateMap.emplace(clausevector[i]->name, position);
		}
		else
		{
			predicateMap[clausevector[i]->name].push_back(i);
		}
	}
}

void Clause::negate()
{
	for(auto pre:clausevector)
		pre->positive = (pre->positive+1)%2;
}

void Clause::addPredicate(Predicate* pre)
{
	this->clausevector.push_back(pre);
	if (predicateMap.find(pre->name) == predicateMap.end())
	{
		vector<int> position;
		position.push_back(this->clausevector.size()-1);
		predicateMap.emplace(pre->name, position);
	}
	else
	{
		predicateMap[pre->name].push_back(this->clausevector.size()-1);
	}
	
}

void Clause::clear()
{
	this->clausevector.clear();
}

bool Clause::equals(Clause* B)
{
	vector<Predicate*> preVectorA = this->clausevector;
	vector<Predicate*> preVectorB = B->getClausevector();
	if(preVectorA.size()!=preVectorB.size())
		return false;
	else
	{
		for(auto preA : preVectorA)
		{
			//find the same name
			if(B->predicateMap.find(preA->name) == B->predicateMap.end())
				return false;
			else
			{
				vector<int> preNo = B->predicateMap[preA->name];
				int hasBreak = 0;
				for(auto no:preNo)
				{
					for(int i = 0; i < preA->variable.size(); i++)
					{
						if (preA->positive != preVectorB[no]->positive)
						{
							hasBreak++;
							break;
						}
						else if(preA->variable[i].compare(preVectorB[no]->variable[i])!=0)
						{
							hasBreak++;
							break;	
						}
					}
				}
				if(hasBreak!=0 && hasBreak == preNo.size())
					return false;
			}
		}
	}
//	cout << "equal:";
//	this->printClause();
//	cout << " and ";
//	B->printClause();
//	cout << endl;
	return true;
}

void Clause::printClause()
{
	for(auto pre : clausevector)
	{
		cout<<pre->name<<"(";
		for(auto var : pre->variable)
		{
			cout<<var<<",";
		}
		cout<<")";
	}
}
