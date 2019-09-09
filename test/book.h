#ifndef _Book_H_
#define _Book_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class Book :public ObjectMap {
		private:
		public:
		static void Initialize(){ORM* model=ORM::GetInstance(); std::map<std::string,std::string> hashmap; CreateTable(hashmap); model->Create(GetTableName(), hashmap); unsigned long maxid = model->MaxPrimaryKey(GetTableName(),GetPrimaryKey()); ObjectMap::Initialize(GetTableName(),maxid);}
		static std::string GetTableName() { return "Book";	};
		static std::string GetPrimaryKey() { return "id"; }
		static void CreateTable(std::map<std::string,std::string> &hashmap){hashmap["id"]= "  INTEGER PRIMARY KEY NOT NULL ";hashmap["Name"]= "  VARCHAR(128) NOT NULL ";hashmap["Author"]= "  INTEGER NOT NULL ";hashmap["Year"]= "  INTEGER ";}
		static long Getid(ObjectMap& x) {return atol(x.Get("id").c_str());}
		static long Getid(ObjectMap* x) {return atol(x->Get("id").c_str());}
		static std::string GetName(ObjectMap& x) {return x.Get("Name");}
		static std::string GetName(ObjectMap* x) {return x->Get("Name");}
		static long GetAuthor(ObjectMap& x) {return atol(x.Get("Author").c_str());}
		static long GetAuthor(ObjectMap* x) {return atol(x->Get("Author").c_str());}
		static long GetYear(ObjectMap& x) {return atol(x.Get("Year").c_str());}
		static long GetYear(ObjectMap* x) {return atol(x->Get("Year").c_str());}


		Book():ObjectMap(GetTableName()) {}
		Book(std::string Name_,long Author_,long Year_):ObjectMap(GetTableName()){SetName(Name_);SetAuthor(Author_);SetYear(Year_);}
		Book(std::map<std::string,std::string> &hashmap ):ObjectMap(hashmap){}
		Book(Book& x):ObjectMap(x){}
		Book(ObjectMap& x):ObjectMap(x){}
		Book& operator= (Book& x) {ObjectMap::operator=(x); return *this;}
		std::string GetTableNameString() const { return "Book";	};
		std::string GetPrimaryKeyString() const { return "id"; }
		void SetMap(std::map<std::string,std::string> &hashmap ){Set("id", hashmap["id"]);Set("Name", hashmap["Name"]);Set("Author", hashmap["Author"]);Set("Year", hashmap["Year"]);}
		void GetMap(std::map<std::string,std::string> &hashmap ) const {hashmap["id"]=Get("id");hashmap["Name"]=Get("Name");hashmap["Author"]=Get("Author");hashmap["Year"]=Get("Year");}
		unsigned long GetId() const {return atol(Get("id").c_str());}
		void SetId(unsigned long variable_) {Set("id",toString(variable_));}
		std::string GetName() const {return Get("Name");}
		void SetName(std::string variable_) {Set("Name",variable_);}
		long GetAuthor() const {return atol(Get("Author").c_str());}
		void SetAuthor(long variable_) {Set("Author",toString(variable_));}
		long GetYear() const {return atol(Get("Year").c_str());}
		void SetYear(long variable_) {Set("Year",toString(variable_));}

	};
};
#endif //_Book_H_

