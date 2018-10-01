#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <list>
//#include <sqlite3.h>
#include "resultset.h"
#include "debug.h"

class sqlite3;

namespace PlusORM {

	class DatabaseAbstract{
	protected:

	public:
		DatabaseAbstract();
		virtual ~DatabaseAbstract();
		virtual bool Status()=0;
		virtual bool Create(std::string table, std::map<std::string,std::string>& hashmap)=0;
		virtual bool Insert(std::string table, std::map<std::string,std::string>& hashmap)=0;
		virtual bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition)=0;
		virtual bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows )=0;
		virtual bool Remove(std::string table, std::string condition)=0;
		virtual bool Drop(std::string table)=0;
	};

	class SqLight : public DatabaseAbstract {
	private:
		sqlite3 *db;
		std::string dbName;

		bool Open();
		bool Close();
		//	const char *sql, int (*sqlite_callback)(void*,int,char**,char**) , void *data, char **errmsg
		bool Do(const char *sql,ResultSet* rows=NULL);
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);
		void Free(char* errMsg);
		const char* ErrMsg();

	public:
		SqLight(const std::string& name );
		~SqLight();
		bool Status();
		bool Create(std::string table, std::map<std::string,std::string>& hashmap);
		bool Insert(std::string table, std::map<std::string,std::string>& hashmap);
		bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition);
		bool Remove(std::string table,std::string condition);
		bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows);
		bool Drop(std::string table);
	};

	class FileIO :public DatabaseAbstract {
		std::string dbName;
	public:
		FileIO(const std::string& name ){dbName=name;}
		~FileIO(){};
		bool Status() {return true;};
		bool Create(std::string table, std::map<std::string,std::string>& hashmap) {return true;};
		bool Insert(std::string table, std::map<std::string,std::string>& hashmap) {return true;};
		bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition) {return true;};
		bool Remove(std::string table,std::string condition) {return true;};
		bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows) {return true;};
		bool Drop(std::string table) {return true;};
	};

	class DBFactory {
	public:
		static DatabaseAbstract* Connect();
		static void Disconnect(DatabaseAbstract* db);
	};

}

#endif //_DATABASE_H_

