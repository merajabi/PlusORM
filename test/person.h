#ifndef _Person_H_
#define _Person_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class Person :public ObjectMap {
		private:
		public:
		static void Initialize(){ORM* model=ORM::GetInstance(); std::map<std::string,std::string> hashmap; CreateTable(hashmap); model->Create(GetTableName(), hashmap); unsigned long maxid = model->MaxPrimaryKey(GetTableName(),GetPrimaryKey()); ObjectMap::Initialize(GetTableName(),maxid);}
		static std::string GetTableName() { return "Person";	};
		static std::string GetPrimaryKey() { return "Id"; }
		static std::string GenKey(const std::string& key) {return (GetTableName()+"#"+key);}
		static std::string ExtKey(const std::string& key) {std::size_t found = key.find(GetTableName()+"#"); if(found!=std::string::npos){ return key.substr(found+GetTableName().size()+1);}	return key; }		

		static void CreateTable(std::map<std::string,std::string> &hashmap){hashmap["Id"]= "  INTEGER PRIMARY KEY NOT NULL ";hashmap["First"]= "  VARCHAR(128) NOT NULL ";hashmap["Last"]= "  VARCHAR(128) NOT NULL ";hashmap["Age"]= "  INTEGER ";}
		static long GetId(ObjectMap& x) {return atol(x.Get(GenKey("Id")).c_str());}
		static long GetId(ObjectMap* x) {return atol(x->Get(GenKey("Id")).c_str());}
		static std::string GetFirst(ObjectMap& x) {return x.Get(GenKey("First"));}
		static std::string GetFirst(ObjectMap* x) {return x->Get(GenKey("First"));}
		static std::string GetLast(ObjectMap& x) {return x.Get(GenKey("Last"));}
		static std::string GetLast(ObjectMap* x) {return x->Get(GenKey("Last"));}
		static long GetAge(ObjectMap& x) {return atol(x.Get(GenKey("Age")).c_str());}
		static long GetAge(ObjectMap* x) {return atol(x->Get(GenKey("Age")).c_str());}


		Person():ObjectMap() {} // No Increase to tableMap
		Person(std::string First_,std::string Last_,long Age_):ObjectMap(GetTableName()){SetId(GetMaxPrimaryKey(GetTableName()));SetFirst(First_);SetLast(Last_);SetAge(Age_);}
		Person(std::map<std::string,std::string> &hashmap ):ObjectMap(hashmap){}
		Person(const Person& x):ObjectMap(x){}
		Person(const ObjectMap& x):ObjectMap(x){}
		Person& operator= (const Person& x) {ObjectMap::operator=(x); return *this;}
		std::string GetTableNameString() const { return "Person";	};
		std::string GetPrimaryKeyString() const { return "Id"; }
		std::string GetPrimaryValueString() const { return Get(GenKey("Id"));}
		void SetMap(std::map<std::string,std::string> &hashmap ){Set(GenKey("Id"), hashmap["Id"]);Set(GenKey("First"), hashmap["First"]);Set(GenKey("Last"), hashmap["Last"]);Set(GenKey("Age"), hashmap["Age"]);}
		void GetMap(std::map<std::string,std::string> &hashmap ) const {hashmap["Id"]=Get(GenKey("Id"));hashmap["First"]=Get(GenKey("First"));hashmap["Last"]=Get(GenKey("Last"));hashmap["Age"]=Get(GenKey("Age"));}
		long GetId() const {return atol(Get(GenKey("Id")).c_str());}
		void SetId(long variable_) {Set(GenKey("Id"),toString(variable_));}
		std::string GetFirst() const {return Get(GenKey("First"));}
		void SetFirst(std::string variable_) {Set(GenKey("First"),variable_);}
		std::string GetLast() const {return Get(GenKey("Last"));}
		void SetLast(std::string variable_) {Set(GenKey("Last"),variable_);}
		long GetAge() const {return atol(Get(GenKey("Age")).c_str());}
		void SetAge(long variable_) {Set(GenKey("Age"),toString(variable_));}

	};
};
#endif //_Person_H_

