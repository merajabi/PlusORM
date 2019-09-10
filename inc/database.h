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
	virtual bool Status()=0;
	virtual bool Create(std::string table, std::map<std::string,std::string>& hashmap)=0;
	virtual bool Insert(std::string table, std::map<std::string,std::string>& hashmap)=0;
	virtual bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition)=0;
	virtual bool Query(const std::string& query, ResultSet& rows){return false;}
	virtual bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows )=0;
	virtual bool Join(const std::string& table1,const std::string& key1,const std::string& table2, const std::string& key2,const std::string& joinType,bool null, ResultSet& rows){return false;};
	virtual bool Remove(std::string table, std::string condition)=0;
	virtual bool Drop(std::string table)=0;
	virtual ~DatabaseAbstract(){}
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
	SqLight(const std::string& name ){
		dbName=name;
		posDebug("SqLight::SqLight() dbName=%s\n",dbName.c_str());
		Open();
	}
	~SqLight(){
		posDebug("SqLight::~SqLight() dbName=%s\n",dbName.c_str());
		Close();
	}
	bool Status(){
		posDebug("SqLight::Status() ErrMsg:%s:\n",ErrMsg());
		return (strcmp(ErrMsg(),"not an error")==0)?true:false;
	}
	bool Create(std::string table, std::map<std::string,std::string>& hashmap) {
		std::string lines;

		for (std::map<std::string,std::string>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it){
			lines	+= "`" + it->first  + "` " + it->second+ ",";
		}

		lines = lines.substr(0, lines.size()-1);

		std::string query = "CREATE TABLE IF NOT EXISTS `" + table + "` ( " + lines + ");";
		return Do(query.c_str());
	}
	bool Insert(std::string table, std::map<std::string,std::string>& hashmap){
		std::string keys;
		std::string values;

		for (std::map<std::string,std::string>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it){
			keys	+= "`" + it->first  + "`" + ",";
			values	+= "\"" + it->second + "\"" + ",";
		}

		keys = keys.substr(0, keys.size()-1);
		values = values.substr(0, values.size()-1);

		std::string query = "INSERT INTO `" + table + "` ( " + keys + " ) VALUES ( " + values + " )";
		return Do(query.c_str());
	}

	bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition){
		std::string pairs;

		for (std::map<std::string,std::string>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it){
			pairs	+= " `" + it->first  + "`" + " = " + "\"" + it->second + "\"" + " ,";
		}

		pairs = pairs.substr(0, pairs.size()-1);

		std::string query =	"UPDATE `" + table + "` SET " + pairs + " WHERE " + condition;
		return Do(query.c_str());
	}
	bool Remove(std::string table,std::string condition){
		std::string query = "DELETE FROM `" + table + "` WHERE "+condition;
		return Do(query.c_str());
	}
	bool Query(const std::string& query, ResultSet& rows){
		return Do(query.c_str(),&rows);
	}
	bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows){
		std::string query = "SELECT '"+table+"' as "+table+", "+ elements + " FROM `" + table + "` WHERE "+condition;
		return Do(query.c_str(),&rows);
	}
	bool Join(const std::string& table1,const std::string& key1,const std::string& table2, const std::string& key2,const std::string& joinType,bool exclude, ResultSet& rows){
		std::string query = "SELECT '"+table1+"' as "+table1+", a.*, '"+table2+"' as "+table2+", b.* FROM "+table1+" as a "+joinType+" JOIN "+table2+" as b ON a."+key1+" = b."+key2;
		if(exclude){
			if(joinType=="LEFT"){
				query += " WHERE b."+key2+" IS NULL";
			}else if (joinType=="RIGHT"){
				query += " WHERE a."+key1+" IS NULL";
			}else if (joinType=="FULL OUTER"){
				query += " WHERE a."+key1+" IS NULL OR b."+key2+" IS NULL";
			}
		}
		return Do(query.c_str(),&rows);
	}
	bool Drop(std::string table) {
		std::string query = "DROP TABLE `" + table + "`";
		return Do(query.c_str());
	}
};
class FileIO :public DatabaseAbstract {
	std::string dbName;
public:
	FileIO(const std::string& name ){
		posDebug("FileIO::FileIO() dbName=%s\n",name.c_str());
		dbName=name;
	}

	bool Status() {return true;}
	bool Create(std::string table, std::map<std::string,std::string>& hashmap) {return true; };
	bool Insert(std::string table, std::map<std::string,std::string>& hashmap) {return true; };
	bool Select(std::string table, std::string elements, std::string condition, ResultSet& rows){return true; };
	bool Count (std::string table, ResultSet& rows) {return true; };
	bool Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition) {return true; };
	bool Remove(std::string table,std::string condition) {return true; };

	bool Drop(std::string table) {return true; };
};

class DBFactory {
public:
	static DatabaseAbstract* Connect();
	static void Disconnect(DatabaseAbstract* db);
};

}

#endif //_DATABASE_H_

