#include "orm.h"
#include "debug.h"

using namespace PlusORM;
std::map<std::string,unsigned long> ObjectMap::tableMap;

//ObjectMap(){id=0;}
ObjectMap::ObjectMap(const ObjectMap& x){
	id=x.id;
}
ObjectMap::ObjectMap(unsigned long xid){
	id=xid;
}
ObjectMap::ObjectMap(const std::string& tableName){
	id=tableMap[tableName]++;
	posDebug("#910 ObjectMap::ObjectMap() tableMap[ %s ]: %d\n",tableName.c_str(),id);
}
ObjectMap& ObjectMap::operator= (const ObjectMap& x) {
	id=x.id;
	return *this;
}
ObjectMap& ObjectMap::operator= (unsigned long xid) {
	id=xid;
	return *this;
}
ObjectMap& ObjectMap::operator= (const std::string& tableName){
	id=tableMap[tableName]++;
	posDebug("#910 ObjectMap::operator=() tableMap[ %s ]: %d\n",tableName.c_str(),id);
	return *this;
}

void ObjectMap::Initialize(const std::string& tableName,unsigned long maxid){
	tableMap[tableName]=++maxid;
}

