#include "global.h"
#include "FileUtil.h"
#include "Scan.h"
#include "Clause.h"
#include "Predicate.h"
#include "Resolution.h"
#include "stdlib.h"

int main()
{
	FileUtil file;
	vector<string> fileStr;
	fileStr = file.loadFile("input.txt");
	Scan scanner;
	vector<Clause*> KB;
	vector<Clause*> query;
	query = scanner.getQuery(fileStr);
	KB = scanner.getKB(fileStr);
	Resolution resolution;
	vector<bool> result;
	for (int i = 0; i < query.size(); i++)
	{
		bool res = resolution.doResolution(KB, query[i]);
		result.push_back(res);
	}
	for (auto res : result)
		cout << res << endl;
	system("PAUSE");
	return 0;
}
