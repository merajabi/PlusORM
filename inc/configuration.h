#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
#include <string>
class Configuration {
public:
	enum DBTypes {FILEIO,SQLITE,NOSQL};
private:
	static DBTypes dbType;
	static std::string dbName;

public:
	static DBTypes DBType(){return dbType;};
	static std::string DBName(){return dbName;};
};
#endif //_CONFIGURATION_H_
