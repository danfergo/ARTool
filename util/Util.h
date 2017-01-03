//
// Created by danfergo on 02-11-2016.
//

#ifndef DISPLAYIMAGE_UTIL_H
#define DISPLAYIMAGE_UTIL_H

#include <string>
#include <vector>

class Util {
public:
	static std::vector<std::string> split(const std::string &s, char delim);
	static std::vector<std::string> fileNamesAt(std::string path);

};


#endif //DISPLAYIMAGE_UTIL_H
