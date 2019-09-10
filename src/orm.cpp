#include "orm.h"
#include "debug.h"

using namespace PlusORM;
std::map<std::string,unsigned long> ObjectMap::tableMap;
/*
ObjectMap::ObjectMap(unsigned long xid){
	obj.insert(std::pair<std::string,std::string>("id",toString(xid)));
	//id=xid;
}
*/
ObjectMap::ObjectMap(const std::string& tableName){
	tableMap[tableName]++;
	//obj.insert(std::pair<std::string,std::string>("id",toString(tmpid)));
	//posDebug("#910 ObjectMap::ObjectMap() tableMap[ %s ]: %d\n",tableName.c_str(),tmpid);
}
ObjectMap::ObjectMap(std::map<std::string,std::string> &hashmap ):obj(hashmap.begin(),hashmap.end()){
	tableMap[GetTableNameString()]++;
	//obj.insert(std::pair<std::string,std::string>("id",toString(tmpid)));
}
//ObjectMap(){id=0;}
ObjectMap::ObjectMap(const ObjectMap& x):obj(x.obj.begin(),x.obj.end()){
	//id=x.id;
}

ObjectMap& ObjectMap::operator= (const ObjectMap& x) {
	//id=x.id;
	obj.insert(x.obj.begin(),x.obj.end());
	return *this;
}
/*
ObjectMap& ObjectMap::operator= (unsigned long xid) {
	id=xid;
	return *this;
}
*/
/*
ObjectMap& ObjectMap::operator= (const std::string& tableName){
	unsigned long tmpid=tableMap[tableName]++;
	obj.insert(std::pair<std::string,std::string>("id",toString(tmpid)));
	//posDebug("#910 ObjectMap::operator=() tableMap[ %s ]: %d\n",tableName.c_str(),tmpid);
	return *this;
}
*/
void ObjectMap::Initialize(const std::string& tableName,unsigned long maxid){
	tableMap[tableName]=maxid;
}

