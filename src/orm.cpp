#include "orm.h"

using namespace PlusORM;

std::map<std::string,unsigned long> ObjectMap::tableMap;

//ObjectMap(){id=0;}
ObjectMap::ObjectMap(ObjectMap& x){
	id=x.id;
}
ObjectMap::ObjectMap(unsigned long xid){
	id=xid;
}
ObjectMap::ObjectMap(std::string tableName){
	id=tableMap[tableName]++;
	posDebug("ObjectMap::ObjectMap() tableMap[ %s ]: %lu\n",tableName.c_str(),id);
}
ObjectMap& ObjectMap::operator= (ObjectMap& x) {
	id=x.id;
	return *this;
}
ObjectMap& ObjectMap::operator= (unsigned long xid) {
	id=xid;
	return *this;
}
ObjectMap& ObjectMap::operator= (std::string tableName){
	id=tableMap[tableName]++;
	posDebug("ObjectMap::operator=() tableMap[ %s ]: %lu\n",tableName.c_str(),id);
	return *this;
}

void ObjectMap::Initialize(std::string tableName,unsigned long maxid){
	tableMap[tableName]=++maxid;
}

