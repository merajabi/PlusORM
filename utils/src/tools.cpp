#include <cstdio>
#include "tools.h"
#include "posdebug.h"

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
/*
std::string toString(long i){
	char str[128];
	posDebug("********            toString(long) \n");
	sprintf(str,"%ld",i);
	return str;
}
std::string toString(double i){
	char str[128];
	posDebug("********           toString(double) \n");
	sprintf(str,"%.lf",i);
	return str;
}
*/

template<>
std::string toString(const std::string& value) {
	return value;
}

template<>
std::string toString(const char* value) {
	return value;
}

void MyPosDebug(const char* format, ...){
}
