#ifndef _SCAN_H_
#define _SCAN_H_

#include "global.h"
#include "Predicate.h"
#include "Clause.h"

class TreeNode
{
public:
	bool isOp;
	Predicate* pre;
	int optr;
	vector<TreeNode*> child;
public:
	TreeNode()
	{
		vector<TreeNode*> children(2,NULL);
		child = children;
		isOp = false;
		pre = NULL;
		optr = -100;
	}
	
};


class Scan
{
public:
	vector<Clause*> getQuery(vector<string>& fileString);
	int getQueryNum(vector<string>& fileString);
	int getSentenceNum(vector<string>& fileString);
	vector<Clause*> getKB(vector<string>& fileString);
	Predicate* toPredicate(string query);
	TreeNode* parse(string& sentence);
vector<Clause*> sentenceToClause(string sentence);
void convertToCNF(TreeNode* syntaxTree);
void deleteDoubleNot(TreeNode* syntaxTree);
void eliminateImply(TreeNode* syntaxTree);
void implyNot(TreeNode* syntaxTree);
void eliminateNot(TreeNode* syntaxTree);
void distribution(TreeNode* syntaxTree);
void treeToClauses(TreeNode* syntaxTree,vector<Clause*>& clauses,Clause* clause);
void printTree(TreeNode* syntaxTree);
};


#endif
