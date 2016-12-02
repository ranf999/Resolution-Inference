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
	vector<string> output;
	Timer timer;
	for (int i = 0; i < query.size(); i++)
	{
		timer.start = chrono::system_clock::now();
		bool res = resolution.doResolution(KB, query[i], timer.start);
		result.push_back(res);
	}
	for (auto res : result)
	{
		cout << res << endl;
		if (res == true)
			output.push_back("TRUE");
		else
			output.push_back("FALSE");
	}
		

	file.writeFile("output.txt", output);
	system("PAUSE");
	return 0;
}
