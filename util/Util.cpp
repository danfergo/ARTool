//
// Created by danfergo on 02-11-2016.
//

#include <dirent.h>
#include "Util.h"

using namespace std;

vector<string> Util::fileNamesAt(string path)
{
	DIR *dir;
	dir = opendir(path.c_str());
	struct dirent *ent;
	int cont = 0;
	vector<string> returnVector;
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string aux = ent->d_name;
			if (aux != "." && aux != ".." && aux != "Thumbs.db")
			{
				string imgPath(path + "/" + ent->d_name);
				returnVector.push_back(imgPath);
			}
		}
		closedir(dir);
	}

	return returnVector;
}
