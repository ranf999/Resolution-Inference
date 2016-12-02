#include "Scan.h"

vector<Clause*> Scan::getQuery(vector<string>& fileString)
{
	vector<Clause*> querySet;
	vector<Predicate*> predicates;
	int qNum = getQueryNum(fileString);
	for(int i = 1; i <= qNum; i++)
	{
		string query = fileString[i];
		Clause* clause = new Clause();
		predicates.push_back(toPredicate(query));
		clause->setClausevector(predicates);
		querySet.push_back(clause);
		predicates.clear();
	}
	return querySet;
}

vector<Clause*> Scan::getKB(vector<string>& fileString)
{ 
	int qNum = getQueryNum(fileString);
	int sNum = getSentenceNum(fileString);
	int i = qNum + 2;
	vector<Clause*> KB;
	for(i ; i < fileString.size(); i++)
	{
		if (fileString[i].size() == 0) continue;
		vector<Clause* > clauses = sentenceToClause(fileString[i]);
		for(auto clause:clauses)
			KB.push_back(clause);
	}
	return KB;
}

Predicate* Scan::toPredicate(string query)
{
	string name,var;
	vector<string> variable;
	int i = 0;
	bool isPositive = true;
	if(query[0]=='~'){
		i++;
		isPositive = false;
	}
	for(i; i < query.size(); i++)
	{
		if(query[i] == '(')
		{
			i++;
			break;
		}
		
		name.push_back(query[i]);
	}
	for(i; i < query.size(); i++)
	{
		if(query[i] == ')')
		{
			variable.push_back(var);
			break;
		}
		if(query[i] != ',')
			var.push_back(query[i]);
		else 
		{
			variable.push_back(var);
			var.clear();
		}
	}
	Predicate* pre = new Predicate(name, variable, isPositive);
	return pre;
}

int Scan::getQueryNum(vector<string>& fileString)
{
	int num = atoi(fileString[0].c_str());
	return num;
}

int Scan::getSentenceNum(vector<string>& fileString)
{
	int num, queryNum;
	queryNum = getQueryNum(fileString);
	num = atoi(fileString[queryNum+1].c_str());
	return num;
}

vector<Clause*> Scan::sentenceToClause(string sentence)
{
	vector<Clause*> clauses;
	TreeNode* syntaxTree;
	syntaxTree = parse(sentence);
	convertToCNF(syntaxTree);
	Clause* clause = new Clause();
	treeToClauses(syntaxTree,clauses,clause);
	vector<Predicate*> preVector = clause->getClausevector();
	vector<Predicate*> tmpVector;	
	for (int i = 0; i < preVector.size(); i++)
	{
		if (preVector[i]->name.compare("NULLPREDICATE") !=0 )
		{
			tmpVector.push_back(preVector[i]);
		}
		else
		{
			Clause* tmp = new Clause(tmpVector);
			clauses.push_back(tmp);
			tmpVector.clear();
		}
	}
	Clause* tmp = new Clause(tmpVector);
	clauses.push_back(tmp);
	tmpVector.clear();

	//clauses.push_back(clause);
	return clauses;
} 

TreeNode* Scan::parse(string& sentence)
{ 
	stack<int> opt;
	stack<TreeNode*> opr;
	int lparenCnt = 0;
	TreeNode* tmp = new TreeNode(); 
	for(int i = 0; i < sentence.size(); i++)
	{
		if(sentence[i] == '(')
		{ 
			opt.push(LPAREN);
			lparenCnt++;
		}
		else if(sentence[i] == '~')
			opt.push(NOT);
		else if(sentence[i] == '&')
			opt.push(AND);
		else if(sentence[i] == '|')
			opt.push(OR);
		else if(sentence[i] == '=')
		{
			opt.push(IMPLY);
			i++;
		}
		else if(sentence[i] >= 'A' && sentence[i] <= 'Z')
		{
			string predicate;
			while(sentence[i]!=')')
			{
				predicate.push_back(sentence[i]);
				i++;
		 	}
			predicate.push_back(')');
			Predicate* pre = new Predicate();
			pre = toPredicate(predicate);
			TreeNode* preNode = new TreeNode();
			preNode->isOp = false;
			preNode->pre = pre;
			opr.push(preNode);
		} 
		else if(sentence[i] == ')')
		{
			int op = opt.top();
			opt.pop();
			TreeNode* node = new TreeNode();
			node->isOp = true;
			node->optr = op;
			if(op == NOT)
			{
				TreeNode* childNode = opr.top();
				opr.pop();
				node->child[0] = childNode;
			}
			else if(op == OR || op == AND || op == IMPLY)
			{
				TreeNode* right = opr.top();
				opr.pop();
				TreeNode* left = opr.top();
				opr.pop();
				node->child[0] = left;
				node->child[1] = right;
			}
			opt.pop();
			if(opt.size() > 0)
				opr.push(node);
			else
				return node;
		}
	}
	return opr.top();
}

void Scan::convertToCNF(TreeNode* syntaxTree)
{
	//eliminate the IMPLY
	eliminateImply(syntaxTree);
	//eliminate the NOT
	deleteDoubleNot(syntaxTree);
	implyNot(syntaxTree);
	eliminateNot(syntaxTree);
	//distribution law
	distribution(syntaxTree);
	printTree(syntaxTree);
	cout<<endl;
}

void Scan::eliminateImply(TreeNode* syntaxTree)
{ 
	if(syntaxTree)
	{
		if(syntaxTree->isOp && syntaxTree->optr == IMPLY)
		{
			syntaxTree->optr = OR;
			TreeNode* node = new TreeNode();
			node->isOp = true;
			node->optr = NOT;
			TreeNode* tmp = syntaxTree->child[0];
			syntaxTree->child[0] = node;
		    node->child[0] = tmp;	
		}
		eliminateImply(syntaxTree->child[0]);
		eliminateImply(syntaxTree->child[1]);
	}
}

void Scan::deleteDoubleNot(TreeNode* syntaxTree)
{
	if (syntaxTree)
	{
		if (syntaxTree->child[0] && syntaxTree->child[0]->optr == NOT && syntaxTree->child[0]->child[0] && syntaxTree->child[0]->child[0]->optr == NOT)
			syntaxTree->child[0] = syntaxTree->child[0]->child[0]->child[0];
		if (syntaxTree->child[1] && syntaxTree->child[1]->optr == NOT && syntaxTree->child[1]->child[0] && syntaxTree->child[1]->child[0]->optr == NOT)
			syntaxTree->child[1] = syntaxTree->child[1]->child[0]->child[0];
		deleteDoubleNot(syntaxTree->child[0]);
		deleteDoubleNot(syntaxTree->child[1]);
	}
	
}

void Scan::implyNot(TreeNode* syntaxTree)
{ 
	if(syntaxTree)
	{
		if(syntaxTree->isOp && syntaxTree->optr == NOT)
		{
			if(syntaxTree->child[0]&&syntaxTree->child[0]->isOp == false)
				syntaxTree->child[0]->pre->positive = (syntaxTree->child[0]->pre->positive + 1)%2;
			else if(syntaxTree->child[0]&&syntaxTree->child[0]->optr == OR)
			{
				syntaxTree->child[0]->optr = AND;
				TreeNode* left = new TreeNode();
				TreeNode* right = new TreeNode();
				left->isOp = true;
				left->optr = NOT;
				right->isOp = true;
				right->optr = NOT;
				TreeNode* tmp = syntaxTree->child[0]->child[0];
				syntaxTree->child[0]->child[0] = left;
				left->child[0] = tmp;
				tmp = syntaxTree->child[0]->child[1];
				syntaxTree->child[0]->child[1] = right;
				right->child[0] = tmp;
			}
			else if(syntaxTree->child[0]&&syntaxTree->child[0]->optr == AND)
			{
				syntaxTree->child[0]->optr = OR;
				TreeNode* left = new TreeNode();
				TreeNode* right = new TreeNode();
				left->isOp = true;
				left->optr = NOT;
				right->isOp = true;
				right->optr = NOT;
				TreeNode* tmp = syntaxTree->child[0]->child[0];
				syntaxTree->child[0]->child[0] = left;
				left->child[0] = tmp;
				tmp = syntaxTree->child[0]->child[1];
				syntaxTree->child[0]->child[1] = right;
				right->child[0] = tmp;

			}

		}
		implyNot(syntaxTree->child[0]);
		implyNot(syntaxTree->child[1]);
	}
}

void Scan::eliminateNot(TreeNode* syntaxTree)
{ 
	if(syntaxTree)
	{
		if(syntaxTree->child[0]&&syntaxTree->child[0]->optr == NOT)
		{
			syntaxTree->child[0] = syntaxTree->child[0]->child[0];
		}
		if(syntaxTree->child[1]&&syntaxTree->child[1]->optr == NOT)
		{	
			syntaxTree->child[1] = syntaxTree->child[1]->child[0];
		}
		eliminateNot(syntaxTree->child[0]);
		eliminateNot(syntaxTree->child[1]);
	}
}

void Scan::distribution(TreeNode* syntaxTree)
{
	if(syntaxTree)
	{
		if(syntaxTree->optr == OR && syntaxTree->child[0]->optr == AND)
		{
			syntaxTree->optr = AND;
			syntaxTree->child[0]->optr = OR;
			TreeNode* tmp = syntaxTree->child[1];
			TreeNode* node = new TreeNode();
			node->isOp = true;
			node->optr = OR;
			syntaxTree->child[1] = node;
			node->child[0] = syntaxTree->child[0]->child[1];
			node->child[1] = tmp;
			syntaxTree->child[0]->child[1] = tmp; 
		}
		if(syntaxTree->optr == OR && syntaxTree->child[1]->optr == AND)
		{
			syntaxTree->optr == AND;
			syntaxTree->child[1]->optr = OR;
			TreeNode* tmp = syntaxTree->child[0];
			TreeNode* node = new TreeNode();
			node->isOp = true;
			node->optr = OR;
			syntaxTree->child[0] = node;
			node->child[0] = syntaxTree->child[1]->child[0];
			node->child[1] = tmp;
			syntaxTree->child[1]->child[0] = tmp; 
		}
		distribution(syntaxTree->child[0]);
		distribution(syntaxTree->child[1]);
	}
}

void Scan::treeToClauses(TreeNode* syntaxTree,vector<Clause*>& clauses,Clause* clause)
{
	if(syntaxTree)
	{	
		treeToClauses(syntaxTree->child[0], clauses, clause);
		if(syntaxTree->isOp == false)
		{
			clause->addPredicate(syntaxTree->pre);
		}
		if(syntaxTree->isOp == true && syntaxTree->optr == AND)
		{
			
			vector<string> var = { "i"};
			Predicate* nullPre = new Predicate("NULLPREDICATE", var, true);
			clause->addPredicate(nullPre);
		}
		treeToClauses(syntaxTree->child[1], clauses, clause);


	}

}

void Scan::printTree(TreeNode* syntaxTree)
{
	if(syntaxTree)
	{
		printTree(syntaxTree->child[0]);
		if(syntaxTree->isOp == true)
		{
			switch(syntaxTree->optr)
			{
				case NOT : cout<<"~"; break;
				case AND : cout<<"&"; break;
				case OR  : cout<<"|"; break;
				case IMPLY : cout<<"=>"; break;
				default: break;
			}
		}
		else 
		{
			string name = syntaxTree->pre->getName();
			vector<string> args = syntaxTree->pre->getVariable();
			bool positive = syntaxTree->pre->isPositive();
			if (positive == false)
				cout << "~";
			cout<<name<<"(";
			for(auto arg:args)
				cout<<arg<<",";
			cout<<")";
		}
		printTree(syntaxTree->child[1]);
	}
}
