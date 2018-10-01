#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "posdebug.h"

#ifndef _TOOLS_H_
#define _TOOLS_H_

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim);

template<typename T>
std::string toString(const T& value) {
	char str[128];
	sprintf(str,"%.0lf",static_cast<double>(value));
	return str;
}

template<typename T>
std::string toString(const T* value) {
	char str[128];
	sprintf(str,"%.0lf",static_cast<double>(*value));
	return value;
}


template<>
std::string toString(const std::string& value);

template<>
std::string toString(const char* value);

//std::string toString(long i);
//std::string toString(double i);

#endif //_TOOLS_H_

