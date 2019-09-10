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

atomic<ORM*> ORM::instance = nullptr;

mutex ORM::m;

ORM::ORM(){
	//posDebug("ORM::ORM()\n");
	db = DBFactory::Connect();
}

ORM::~ORM(){
	ClearResultList();
	DBFactory::Disconnect(db);
}

ORM* ORM::GetInstance(){
	//posDebug("GetInstance\n");
	if(instance == nullptr){
		lock_guard<mutex> lk(m);
		if(instance == nullptr){
			ORM* tmp = new ORM();
			instance = tmp;
		}
	}
	return instance;
}

bool ORM::RemoveInstance(){
	lock_guard<mutex> lk(m);
	if(instance != nullptr){
		ORM* tmp = instance;
		delete 	tmp;
		instance = nullptr;
	}
	return (instance == nullptr);
}

void ORM::ClearResultList(){
	while (!resultlist.empty()) {
		ObjectMap* ptr=resultlist.front();
		resultlist.pop_front();
		delete ptr;
	}
	return ;
}

bool ORM::Create(const std::string& tableName, std::map<std::string,std::string>& hashmap){
	return db->Create(tableName,hashmap);
}

bool ORM::Drop(const std::string& tableName){
	return db->Drop(tableName);
}

bool ORM::Insert(const ObjectMap& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	return db->Insert(x.GetTableNameString(),hashmap);
}

bool ORM::Insert(std::list<ObjectMap*>& list){
	bool ret=false;
	for(typename std::list<ObjectMap*>::iterator it=list.begin(); it != list.end(); it++){
		ret=Insert(*(*it));
	}
	return ret;
}

bool ORM::Update(const ObjectMap& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	std::string condition=x.GetPrimaryKeyString()+" == "+x.GetPrimaryValueString();
	return db->Update(x.GetTableNameString(),hashmap,condition);
}

bool ORM::Update(std::list<ObjectMap*>& list){
	bool ret=false;
	for(typename std::list<ObjectMap*>::iterator it=list.begin(); it != list.end(); it++){
		ret=Update(*(*it));
	}
	return ret;
}

bool ORM::Remove(const ObjectMap& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	std::string condition=x.GetPrimaryKeyString()+" == "+x.GetPrimaryValueString();
	return db->Remove(x.GetTableNameString(), condition);
}


bool ORM::Remove(const std::string& tableName, const std::string& condition){
	return db->Remove(tableName, condition);
}

bool ORM::Query(const std::string& query){
	bool ret=false;
	ResultSet rows;
	ret = db->Query(query,rows);
	ClearResultList();
	for(ResultSet::iterator itl=rows.begin(); itl != rows.end(); itl++){
		Row* row = *itl;
		ObjectMap* ptr=new ObjectMap(*row);
		resultlist.push_back(ptr);
	}
	return ret;
}

bool ORM::Search(const std::string& tableName, const std::string& elements, const std::string& condition){
	bool ret=false;
	ResultSet rows;
	ret = db->Select(tableName,elements,condition,rows);
	ClearResultList();
	for(ResultSet::iterator itl=rows.begin(); itl != rows.end(); itl++){
		Row* row = *itl;
		ObjectMap* ptr=new ObjectMap(*row);
		resultlist.push_back(ptr);
	}
	return ret;
}

bool ORM::Join(const std::string& table1,const std::string& key1,const std::string& table2, const std::string& key2,const std::string& joinType,bool exclude){
	bool ret=false;
	ResultSet rows;
	ret = db->Join(table1,key1,table2,key2,joinType,exclude,rows);
	ClearResultList();
	for(ResultSet::iterator itl=rows.begin(); itl != rows.end(); itl++){
		Row* row = *itl;
		ObjectMap* ptr=new ObjectMap(*row);
		resultlist.push_back(ptr);
	}
	return ret;
}

unsigned long ORM::Count(const std::string& tableName, const std::string& elements, const std::string& condition){
	ResultSet rows;
	db->Select(tableName,elements,condition,rows);
	Row* row=rows.front();
	//posDebug("%s %s\n",elements.c_str(),(*row)[elements].c_str() );
	return atol((*row)[tableName+"#"+elements].c_str());
}

unsigned long ORM::Count(const std::string& tableName){
	return Count(tableName,"Count(*)","1 == 1");
}

unsigned long ORM::MaxPrimaryKey(const std::string& tableName,const std::string& primaryKeyString){
	std::string element="MAX(`"+primaryKeyString+"`)";
	return Count(tableName,element,"1 == 1");
}


