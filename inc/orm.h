#ifndef _ORM_H_
#define _ORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <list>
#include <pthread.h>

#include "debug.h"
#include "resultset.h"
#include "database.h"

namespace PlusORM {

class DatabaseAbstract;

class ObjectMap {
protected:
	static std::map<std::string,unsigned long> tableMap;
	unsigned long id;
public:
	//ObjectMap(){id=0;}
	virtual ~ObjectMap(){}
	ObjectMap(ObjectMap& x);
	ObjectMap(unsigned long xid);
	ObjectMap(std::string tableName);
	ObjectMap& operator= (ObjectMap& x);
	ObjectMap& operator= (unsigned long xid);
	ObjectMap& operator= (std::string tableName);
	static void Initialize(std::string tableName,unsigned long maxid);
	//static std::string GetPrimaryKeyString();
	//static std::string GetTableName();
	inline virtual void SetMap(std::map<std::string,std::string> &hashmap ) = 0 ;
	inline virtual void GetMap(std::map<std::string,std::string> &hashmap ) const = 0;

	std::string GetPrimaryValueString() const { return toString<unsigned long>(id); }
};

template <typename T>
class ORM {
	static pthread_mutex_t mutex;
	static ORM* volatile instance;
	std::list<T*> resultlist;
	DatabaseAbstract* db;
    ORM<T>();
    ~ORM<T>();
	void ClearResultList();
public:
	static ORM* volatile GetInstance();
	static bool RemoveInstance();
	bool Status() const {return db->Status();}
	bool Create();
	bool Drop();
	bool Insert(const T& x);
	bool Insert(const std::list<T*>& list);
	bool Search(std::string elements="*", std::string condition = "1==1");
	bool Update(const T& x);
	bool Update(const std::list<T*>& list);
	bool Remove(const T& x);
	bool Remove(std::string condition);
	unsigned long Count(std::string elements, std::string condition) const;
	unsigned long Count() const;
	unsigned long MaxPrimaryKey() const;
	const std::list<T*>& GetResultList() const { return resultlist;}

};

template <typename T>
ORM<T>* volatile ORM<T>::instance = nullptr;

template <typename T>
pthread_mutex_t ORM<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <typename T>
ORM<T>::ORM(){
	posDebug("ORM<T>::ORM()\n");
	db = DBFactory::Connect();
	Create();
}
template <typename T>
ORM<T>::~ORM(){
	posDebug("ORM<T>::~ORM()\n");
	ClearResultList();
	DBFactory::Disconnect(db);
}

template <typename T>
ORM<T>* volatile ORM<T>::GetInstance(){
	posDebug("ORM<T>::GetInstance()\n");
	if(instance == nullptr){
		pthread_mutex_lock(&mutex);
		if(instance == nullptr){
			posDebug(" new ORM<T>()\n");
			ORM<T>* volatile temp = new ORM<T>();
			instance = temp;
		}
		pthread_mutex_unlock(&mutex);
	}
	return instance;
}

template <typename T>
bool ORM<T>::RemoveInstance(){
	posDebug("ORM<T>::RemoveInstance()\n");
	if(instance != nullptr){
		pthread_mutex_lock(&mutex);
		if(instance != nullptr){
			posDebug(" delete 	instance\n");
			delete 	instance;
			instance = nullptr;
		}
		pthread_mutex_unlock(&mutex);
	}
	return (instance == nullptr);
}

template <typename T>
void ORM<T>::ClearResultList(){
	while (!resultlist.empty()) {
		T* ptr=resultlist.front();
		resultlist.pop_front();
		delete ptr;
	}
	return ;
}

template <typename T>
bool ORM<T>::Create(){
	posDebug("ORM<T>::Create()\n");
	std::map<std::string,std::string> hashmap;
	T::CreateTable(hashmap);
	return db->Create(T::GetTableName(),hashmap);
}

template <typename T>
bool ORM<T>::Drop(){
	return db->Drop(T::GetTableName());
}
template <typename T>
bool ORM<T>::Insert(const T& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	return db->Insert(T::GetTableName(),hashmap);
}

template <typename T>
bool ORM<T>::Insert(const std::list<T*>& list){
	bool ret=false;
	for(typename std::list<T*>::iterator it=list.begin(); it != list.end(); it++){
		ret=Insert(*(*it));
	}
	return ret;
}
template <typename T>
bool ORM<T>::Update(const T& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	std::string condition=T::GetPrimaryKeyString()+" == "+x.GetPrimaryValueString();
	return db->Update(T::GetTableName(),hashmap,condition);
}
template <typename T>
bool ORM<T>::Update(const std::list<T*>& list){
	bool ret=false;
	for(typename std::list<T*>::iterator it=list.begin(); it != list.end(); it++){
		ret=Update(*(*it));
	}
	return ret;
}
template <typename T>
bool ORM<T>::Remove(const T& x){
	std::map<std::string,std::string> hashmap;
	x.GetMap(hashmap);
	std::string condition=T::GetPrimaryKeyString()+" == "+x.GetPrimaryValueString();
	return db->Remove(T::GetTableName(), condition);
}

template <typename T>
bool ORM<T>::Remove(std::string condition){
	return db->Remove(T::GetTableName(), condition);
}

template <typename T>
bool ORM<T>::Search(std::string elements, std::string condition) {
	bool ret=false;
	ResultSet rows;
	ret = db->Select(T::GetTableName(),elements,condition,rows);
	ClearResultList();
	for(ResultSet::iterator itl=rows.begin(); itl != rows.end(); itl++){
		Row* row = *itl;
		T* ptr=new T(*row);
		//ptr->SetMap(*row);
		resultlist.push_back(ptr);
	}
	return ret;
}

template <typename T>
unsigned long ORM<T>::Count(std::string elements, std::string condition) const {
	ResultSet rows;
	db->Select(T::GetTableName(),elements,condition,rows);
	Row* row=rows.front();
	posDebug("%s %s\n",elements.c_str(),(*row)[elements].c_str() );
	return atol((*row)[elements].c_str());
}

template <typename T>
unsigned long ORM<T>::Count() const {
	return Count("Count(*)","1 == 1");
}

template <typename T>
unsigned long ORM<T>::MaxPrimaryKey() const {
	std::string element="MAX(`"+T::GetPrimaryKeyString()+"`)";
	return Count(element,"1 == 1");
}

};

#endif //_ORM_H_

