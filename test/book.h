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
		static std::string GetPrimaryKey() { return "Id"; }
		static std::string GenKey(const std::string& key) {return (GetTableName()+"#"+key);}
		static std::string ExtKey(const std::string& key) {std::size_t found = key.find(GetTableName()+"#"); if(found!=std::string::npos){ return key.substr(found+GetTableName().size()+1);}	return key; }		

		static void CreateTable(std::map<std::string,std::string> &hashmap){hashmap["Id"]= "  INTEGER PRIMARY KEY NOT NULL ";hashmap["Name"]= "  VARCHAR(128) ";hashmap["Author"]= "  INTEGER ";hashmap["Year"]= "  INTEGER ";}
		static long GetId(ObjectMap& x) {return atol(x.Get(GenKey("Id")).c_str());}
		static long GetId(ObjectMap* x) {return atol(x->Get(GenKey("Id")).c_str());}
		static std::string GetName(ObjectMap& x) {return x.Get(GenKey("Name"));}
		static std::string GetName(ObjectMap* x) {return x->Get(GenKey("Name"));}
		static long GetAuthor(ObjectMap& x) {return atol(x.Get(GenKey("Author")).c_str());}
		static long GetAuthor(ObjectMap* x) {return atol(x->Get(GenKey("Author")).c_str());}
		static long GetYear(ObjectMap& x) {return atol(x.Get(GenKey("Year")).c_str());}
		static long GetYear(ObjectMap* x) {return atol(x->Get(GenKey("Year")).c_str());}


		Book():ObjectMap() {}
		Book(std::string Name_,long Author_,long Year_):ObjectMap(GetTableName()){SetId(GetMaxPrimaryKey(GetTableName()));SetName(Name_);SetAuthor(Author_);SetYear(Year_);}
		Book(std::map<std::string,std::string> &hashmap ):ObjectMap(hashmap){}
		Book(const Book& x):ObjectMap(x){}
		Book(const ObjectMap& x):ObjectMap(x){}
		Book& operator= (const Book& x) {ObjectMap::operator=(x); return *this;}
		std::string GetTableNameString() const { return "Book";	};
		std::string GetPrimaryKeyString() const { return "Id"; }
		void SetMap(std::map<std::string,std::string> &hashmap ){Set(GenKey("Id"), hashmap[GenKey("Id")]);Set(GenKey("Name"), hashmap[GenKey("Name")]);Set(GenKey("Author"), hashmap[GenKey("Author")]);Set(GenKey("Year"), hashmap[GenKey("Year")]);}
		void GetMap(std::map<std::string,std::string> &hashmap ) const {hashmap["Id"]=Get(GenKey("Id"));hashmap["Name"]=Get(GenKey("Name"));hashmap["Author"]=Get(GenKey("Author"));hashmap["Year"]=Get(GenKey("Year"));}
		long GetId() const {return atol(Get(GenKey("Id")).c_str());}
		void SetId(long variable_) {Set(GenKey("Id"),toString(variable_));}
		std::string GetName() const {return Get(GenKey("Name"));}
		void SetName(std::string variable_) {Set(GenKey("Name"),variable_);}
		long GetAuthor() const {return atol(Get(GenKey("Author")).c_str());}
		void SetAuthor(long variable_) {Set(GenKey("Author"),toString(variable_));}
		long GetYear() const {return atol(Get(GenKey("Year")).c_str());}
		void SetYear(long variable_) {Set(GenKey("Year"),toString(variable_));}

	};
};
#endif //_Book_H_

