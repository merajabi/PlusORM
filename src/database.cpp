#include <sqlite3.h>
#include <unistd.h>
#include "database.h"
#include "configuration.h"

using namespace PlusORM;

DatabaseAbstract* DBFactory::Connect(){
	posDebug("DBFactory::Create()\n");
	DatabaseAbstract* dbObj;
	switch (Configuration::DBType()) {
		case Configuration::FILEIO:
			dbObj = new FileIO(Configuration::DBName());
			break;
		case Configuration::SQLITE:		
			dbObj = new SqLight(Configuration::DBName());
			break;
		case Configuration::NOSQL:		
			break;
	}
	return dbObj;
}
void DBFactory::Disconnect(DatabaseAbstract* dbObj){
	if(dbObj){
		delete dbObj;
		dbObj=nullptr;
	}
}

DatabaseAbstract::DatabaseAbstract(){
	posDebug("DatabaseAbstract::DatabaseAbstract()\n");
}
DatabaseAbstract::~DatabaseAbstract(){
	posDebug("DatabaseAbstract::~DatabaseAbstract()\n");
}

bool SqLight::Open(){
	posDebug("SqLight::Open()\n");

	//int rc = sqlite3_open(dbName.c_str(),&db);
	//if( rc ) {
	//  posDebug( "Can't open database:%s err: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	//} else {
	//  posDebug( "Opened database successfully:%s msg: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	//}
	//return (rc?false:true);
	

	// SQLITE_OPEN_FULLMUTEX & -DSQLITE_THREADSAFE=1
	while(sqlite3_open_v2(dbName.c_str(),&db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ,NULL)){
		posDebug( "Can't open database:%s err: %s\n",dbName.c_str(), sqlite3_errmsg(db));
		usleep(100);
	}
	posDebug( "Opened database successfully:%s msg: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	return true;
}

bool SqLight::Close(){
	//int rc = sqlite3_close(db);
	int rc = sqlite3_close_v2(db);
	if( rc ) {
	  posDebug( "Can't close database:%d %s\n", rc, sqlite3_errmsg(db));
	} else {
	  posDebug( "closed database successfully:%d \n", rc);
	}
	return (rc?false:true);
}

int SqLight::callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	posDebug("SqLight::callback numcolls: %d\n",argc);
	ResultSet* results=(ResultSet*)data;
	Row *row = new Row;

	for(i = 0; i<argc; i++) {
		//posDebug("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		(*row)[azColName[i]]=argv[i] ? argv[i] : "NULL";
	}
	results->push_back(row);

	posDebug("\n");
	return 0;
}

//	const char *sql, int (*sqlite_callback)(void*,int,char**,char**) , void *data, char **errmsg
bool SqLight::Do(const char *sql,ResultSet* rows){
	char *errMsg = 0;
	posDebug("query: %s\n",sql);
	int rc= sqlite3_exec(db, sql, callback, (void*)rows , &errMsg);
	if( rc != SQLITE_OK ){
	  posDebug( "SQL error: %s\n", errMsg);
	  sqlite3_free(errMsg);
	} else {
	  posDebug( "Query executed successfully %d\n",rc);
	}
	return ((rc == SQLITE_OK)?true:false);
}

void SqLight::Free(char* errMsg){
	sqlite3_free(errMsg);
}

const char* SqLight::ErrMsg(){
	return sqlite3_errmsg(db);
}

SqLight::SqLight(const std::string& name ):db(nullptr),dbName(name){
	posDebug("SqLight::SqLight() dbName=%s\n",dbName.c_str());
	Open();
	posDebug("SqLight::SqLight() dbName=%s\n",dbName.c_str());
}
SqLight::~SqLight(){
	posDebug("SqLight::~SqLight() dbName=%s\n",dbName.c_str());
	Close();
	posDebug("SqLight::~SqLight() dbName=%s\n",dbName.c_str());
}
bool SqLight::Status(){
	posDebug("SqLight::Status() ErrMsg:%s:\n",ErrMsg());
	return (strcmp(ErrMsg(),"not an error")==0)?true:false;
}
bool SqLight::Create(std::string table, std::map<std::string,std::string>& hashmap) {
	std::string lines;

	for (std::map<std::string,std::string>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it){
		lines	+= "`" + it->first  + "` " + it->second+ ",";
	}

	lines = lines.substr(0, lines.size()-1);

	std::string query = "CREATE TABLE IF NOT EXISTS `" + table + "` ( " + lines + ");";
	return Do(query.c_str());
}
bool SqLight::Insert(std::string table, std::map<std::string,std::string>& hashmap){
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

bool SqLight::Update(std::string table, std::map<std::string,std::string>& hashmap, std::string condition){
	std::string pairs;

	for (std::map<std::string,std::string>::iterator it=hashmap.begin(); it!=hashmap.end(); ++it){
		pairs	+= " `" + it->first  + "`" + " = " + "\"" + it->second + "\"" + " ,";
	}

	pairs = pairs.substr(0, pairs.size()-1);

	std::string query =	"UPDATE `" + table + "` SET " + pairs + " WHERE " + condition;
	return Do(query.c_str());
}
bool SqLight::Remove(std::string table,std::string condition){
	std::string query = "DELETE FROM `" + table + "` WHERE "+condition;
	return Do(query.c_str());
}
bool SqLight::Select(std::string table, std::string elements, std::string condition, ResultSet& rows){
	std::string query = "SELECT " + elements + " FROM `" + table + "` WHERE "+condition;
	return Do(query.c_str(),&rows);
}

bool SqLight::Drop(std::string table) {
	std::string query = "DROP TABLE `" + table + "`";
	return Do(query.c_str());
}

