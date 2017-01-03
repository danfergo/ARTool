//
// Created by danfergo on 02-11-2016.
//

#include <dirent.h>
#include "Util.h"
#include <sstream>

using namespace std;

std::vector<std::string> Util::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

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
				string imgPath(ent->d_name);
				returnVector.push_back(imgPath);
			}
		}
		closedir(dir);
	}

	return returnVector;
}
