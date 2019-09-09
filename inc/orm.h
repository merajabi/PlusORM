#ifndef _ORM_H_
#define _ORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <list>

#include "debug.h"
#include "resultset.h"
#include "database.h"
#include "ModernCPP/include/ModernCPP.h"

namespace PlusORM {

	class DatabaseAbstract;

	class ObjectMap {
	protected:
		std::map<std::string,std::string> obj; 
		static std::map<std::string,unsigned long> tableMap;
		//unsigned long id;
	public:
		//ObjectMap(){id=0;}
		ObjectMap(const ObjectMap& x);
		ObjectMap(unsigned long xid);
		ObjectMap(const std::string& tableName);
		ObjectMap(std::map<std::string,std::string> &hashmap );
		virtual ~ObjectMap(){};

		ObjectMap& operator= (const ObjectMap& x);
		//ObjectMap& operator= (unsigned long xid);
		ObjectMap& operator= (const std::string& tableName);

		virtual std::string GetTableNameString() const {return "";};
		virtual std::string GetPrimaryKeyString() const {return "";};

		virtual void SetMap(std::map<std::string,std::string> &hashmap ) {obj.insert(hashmap.begin(),hashmap.end());} ;
		virtual void GetMap(std::map<std::string,std::string> &hashmap ) const {hashmap.insert(obj.begin(),obj.end());};
		
		std::string GetPrimaryValueString() const { return Get("id");/*toString<unsigned long>(id);*/ }
		std::string Get(const std::string& key) const { std::map<std::string,std::string>::const_iterator it = obj.find(key); return ((it!=obj.end())?it->second:"NULL"); }
		void Set(const std::string& key,const std::string& value) {	obj[key]=value;	}

		static void Initialize(const std::string& tableName,unsigned long maxid);
	};

	class ORM {
		static mutex m;
		static atomic<ORM*> instance;
		std::list<ObjectMap*> resultlist;
		DatabaseAbstract* db;
		ORM();
		~ORM();
		void ClearResultList();
	public:
		static ORM* GetInstance();
		static bool RemoveInstance();
		bool Status(){return db->Status();}
		bool Create(const std::string& tableName, std::map<std::string,std::string>& hashmap);
		bool Drop(const std::string& tableName);
		bool Insert(const ObjectMap& x);
		bool Insert(std::list<ObjectMap*>& list);
		bool Search(const std::string& tableName,const std::string& elements="*", const std::string& condition = "1==1");
		bool Update(const ObjectMap& x);
		bool Update(std::list<ObjectMap*>& list);
		bool Remove(const ObjectMap& x);
		bool Remove(const std::string& tableName, const std::string& condition);
		unsigned long Count(const std::string& tableName, const std::string& elements, const std::string& condition);
		unsigned long Count(const std::string& tableName);
		unsigned long MaxPrimaryKey(const std::string& tableName, const std::string& primaryKeyString);
		std::list<ObjectMap*> const& GetResultList() const { return resultlist;}

	};

	atomic<ORM*> ORM::instance = nullptr;

	mutex ORM::m;

	ORM::ORM(){
		//posDebug("ORM::ORM()\n");
		db = DBFactory::Connect();
		//Create();
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
		//posDebug("ORM::Create()\n");
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
	//	std::string condition="id == "+hashmap["id"];
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
	//	std::string condition="id == "+hashmap["id"];
		std::string condition=x.GetPrimaryKeyString()+" == "+x.GetPrimaryValueString();
		return db->Remove(x.GetTableNameString(), condition);
	}


	bool ORM::Remove(const std::string& tableName, const std::string& condition){
		return db->Remove(tableName, condition);
	}


	bool ORM::Search(const std::string& tableName, const std::string& elements, const std::string& condition){
		bool ret=false;
		ResultSet rows;
		ret = db->Select(tableName,elements,condition,rows);
		ClearResultList();
		for(ResultSet::iterator itl=rows.begin(); itl != rows.end(); itl++){
			Row* row = *itl;
			ObjectMap* ptr=new ObjectMap(*row);
			//ptr->SetMap(*row);
			resultlist.push_back(ptr);
		}
		return ret;
	}


	unsigned long ORM::Count(const std::string& tableName, const std::string& elements, const std::string& condition){
		ResultSet rows;
		db->Select(tableName,elements,condition,rows);
		Row* row=rows.front();
		//posDebug("%s %s\n",elements.c_str(),(*row)[elements].c_str() );
		return atol((*row)[elements].c_str());
	}

	unsigned long ORM::Count(const std::string& tableName){
		return Count(tableName,"Count(*)","1 == 1");
	}

	unsigned long ORM::MaxPrimaryKey(const std::string& tableName,const std::string& primaryKeyString){
		std::string element="MAX(`"+primaryKeyString+"`)";
		return Count(tableName,element,"1 == 1");
	}

};

#endif //_ORM_H_

