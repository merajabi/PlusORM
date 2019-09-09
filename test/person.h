#ifndef _Person_H_
#define _Person_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class Person :public ObjectMap {
		private:
		//std::string First;std::string Last;long Age;
		public:
		static void Initialize(){
			ORM* model=ORM::GetInstance();
			std::map<std::string,std::string> hashmap;
			CreateTable(hashmap);
			model->Create(GetTableName(), hashmap);
			unsigned long maxid = model->MaxPrimaryKey(GetTableName(),GetPrimaryKey());
			ObjectMap::Initialize(GetTableName(),maxid);
		}
		static std::string GetTableName() { return "Person";	};
		static std::string GetPrimaryKey() { return "id"; }

		static unsigned long GetId(ObjectMap& x) {return atol(x.Get("id").c_str());}
		static std::string GetFirst(ObjectMap& x) {return x.Get("First");}
		static std::string GetLast(ObjectMap& x) {return x.Get("Last");}
		static long GetAge(ObjectMap& x) {return atol(x.Get("Age").c_str());}

		static unsigned long GetId(ObjectMap* x) {return atol(x->Get("id").c_str());}
		static std::string GetFirst(ObjectMap* x) {return x->Get("First");}
		static std::string GetLast(ObjectMap* x) {return x->Get("Last");}
		static long GetAge(ObjectMap* x) {return atol(x->Get("Age").c_str());}

		static void CreateTable(std::map<std::string,std::string> &hashmap){hashmap["id"]= "  INTEGER PRIMARY KEY NOT NULL ";hashmap["First"]= "  VARCHAR(128) NOT NULL ";hashmap["Last"]= "  VARCHAR(128) NOT NULL ";hashmap["Age"]= "  INTEGER ";}

		Person():ObjectMap(GetTableName()) {}
		Person(std::string First_,std::string Last_,long Age_):ObjectMap(GetTableName()){SetFirst(First_);SetLast(Last_);SetAge(Age_);}
		Person(std::map<std::string,std::string> &hashmap ):ObjectMap(hashmap){}
		Person(Person& x):ObjectMap(x){}
		Person(ObjectMap& x):ObjectMap(x){}
		Person& operator= (Person& x) {ObjectMap::operator=(x); return *this;}

		std::string GetTableNameString() const { return "Person";	};
		std::string GetPrimaryKeyString() const { return "id"; }

		void SetMap(std::map<std::string,std::string> &hashmap ){obj["id"] = hashmap["id"];obj["First"] = hashmap["First"];obj["Last"] = hashmap["Last"];obj["Age"] = hashmap["Age"];}
		void GetMap(std::map<std::string,std::string> &hashmap ) const { hashmap["id"]=Get("id");hashmap["First"]=Get("First");hashmap["Last"]=Get("Last");hashmap["Age"]=Get("Age");}

		unsigned long GetId() {return atol(obj["id"].c_str());}
		void SetId(unsigned long variable_) {obj["id"]=toString(variable_);}
		std::string GetFirst() {return obj["First"];}
		void SetFirst(std::string variable_) {obj["First"]=variable_;}
		std::string GetLast() {return obj["Last"];}
		void SetLast(std::string variable_) {obj["Last"]=variable_;}
		long GetAge() {return atol(obj["Age"].c_str());}
		void SetAge(long variable_) {obj["Age"]=toString(variable_);}

	};
};
#endif //_Person_H_

