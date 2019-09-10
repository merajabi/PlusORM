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
		//ObjectMap(unsigned long xid);
		ObjectMap(const std::string& tableName);
		ObjectMap(std::map<std::string,std::string> &hashmap );
		virtual ~ObjectMap(){};

		ObjectMap& operator= (const ObjectMap& x);
		//ObjectMap& operator= (unsigned long xid);
		//ObjectMap& operator= (const std::string& tableName);

		virtual std::string GetTableNameString() const {return "ObjectMapTable";};
		virtual std::string GetPrimaryKeyString() const {return "ObjectMapPrimaryKey";};
		virtual std::string GetPrimaryValueString() const { return "ObjectMapPrimaryValue";}

		virtual void SetMap(std::map<std::string,std::string> &hashmap ) {obj.insert(hashmap.begin(),hashmap.end());} ;
		virtual void GetMap(std::map<std::string,std::string> &hashmap ) const {hashmap.insert(obj.begin(),obj.end());};

		std::string Get(const std::string& key) const { std::map<std::string,std::string>::const_iterator it = obj.find(key); return ((it!=obj.end())?it->second:"NULL"); }
		void Set(const std::string& key,const std::string& value) {	obj[key]=value;	}

		static void Initialize(const std::string& tableName,unsigned long maxid);
		static unsigned long GetMaxPrimaryKey(const std::string& tableName){return tableMap[tableName];}
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
		bool Query(const std::string& query);
		bool Search(const std::string& tableName,const std::string& elements="*", const std::string& condition = "1==1");
		bool Join(const std::string& table1,const std::string& key1,const std::string& table2, const std::string& key2,const std::string& joinType="INNER",bool exclude=false);
		bool Update(const ObjectMap& x);
		bool Update(std::list<ObjectMap*>& list);
		bool Remove(const ObjectMap& x);
		bool Remove(const std::string& tableName, const std::string& condition);
		unsigned long Count(const std::string& tableName, const std::string& elements, const std::string& condition);
		unsigned long Count(const std::string& tableName);
		unsigned long MaxPrimaryKey(const std::string& tableName, const std::string& primaryKeyString);
		std::list<ObjectMap*> const& GetResultList() const { return resultlist;}
	};

};

#endif //_ORM_H_

