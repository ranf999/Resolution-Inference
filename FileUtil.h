#ifndef _FILEUTIL_H_
#define _FILEUTIL_H_

#include "global.h"
#include <fstream>

using namespace std;

class FileUtil
{
public:
	//FileLoader();
	vector<string> loadFile(string filename);
	void writeFile(string filename, vector<string> file);
};

#endif
