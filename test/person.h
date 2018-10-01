#ifndef _Person_H_
#define _Person_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class Person :public ObjectMap {
		private:
		std::string First;std::string Last;long Age;
		public:
		inline static void Initialize(unsigned long maxid){ObjectMap::Initialize(GetTableName(),maxid);}
		inline static std::string GetTableName() { return "Person";	};
		inline static std::string GetPrimaryKeyString() { return "id"; }
		inline static void CreateTable(std::map<std::string,std::string> &hashmap){hashmap["id"]= "  INTEGER PRIMARY KEY NOT NULL ";hashmap["First"]= "  VARCHAR(128) NOT NULL ";hashmap["Last"]= "  VARCHAR(128) NOT NULL ";hashmap["Age"]= "  INTEGER ";}
		Person():ObjectMap(GetTableName()) {}
		Person(std::string First_,std::string Last_,long Age_):ObjectMap(GetTableName()){First=First_;Last=Last_;Age=Age_;}
		Person(std::map<std::string,std::string> &hashmap ):ObjectMap(GetTableName()){SetMap(hashmap);}
		Person(Person& x):ObjectMap(x){First=x.First;Last=x.Last;Age=x.Age;}
		Person& operator= (Person& x) {ObjectMap::operator=(x);First=x.First;Last=x.Last;Age=x.Age; return *this;}
		inline void SetMap(std::map<std::string,std::string> &hashmap ){id = atol(hashmap["id"].c_str());First = hashmap["First"];Last = hashmap["Last"];Age = atol(hashmap["Age"].c_str());}
		inline void GetMap(std::map<std::string,std::string> &hashmap ) const {char str[128];sprintf(str,"%ld",id);hashmap["id"]=str;hashmap["First"]=First;hashmap["Last"]=Last;sprintf(str,"%ld",Age);hashmap["Age"]=str;}
		unsigned long GetId() const {return id;}
		void SetId(unsigned long variable_) {id=variable_;}
		std::string GetFirst() const {return First;}
		void SetFirst(std::string variable_) {First=variable_;}
		std::string GetLast() const {return Last;}
		void SetLast(std::string variable_) {Last=variable_;}
		long GetAge() const {return Age;}
		void SetAge(long variable_) {Age=variable_;}

	};
};
#endif //_Person_H_

