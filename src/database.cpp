#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "database.h"
#include "configuration.h"

using namespace PlusORM;

DatabaseAbstract* DBFactory::Connect(){
	posDebug("DBFactory::Create()\n");
	DatabaseAbstract* db;
	switch (Configuration::DBType()) {
		case Configuration::FILEIO:
			db = new FileIO(Configuration::DBName());
			break;
		case Configuration::SQLITE:		
			db = new SqLight(Configuration::DBName());
			break;
		case Configuration::NOSQL:		
			break;
	}
	return db;
}
void DBFactory::Disconnect(DatabaseAbstract* db){
	if(db)
		delete db;
}

bool SqLight::Open(){
	posDebug("SqLight::Open()\n");

	//int rc = sqlite3_open(dbName.c_str(),&db);
	//int rc = sqlite3_open_v2(dbName.c_str(),&db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ,NULL);

	while(sqlite3_open_v2(dbName.c_str(),&db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ,NULL)){
		posDebug( "Can't open database:%s err: %s\n",dbName.c_str(), sqlite3_errmsg(db));
		usleep(100);
	}
	/*
	if( rc ) {
	  posDebug( "Can't open database:%s err: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	} else {
	  posDebug( "Opened database successfully:%s msg: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	}
	return (rc?false:true);
	*/
	posDebug( "Opened database successfully:%s msg: %s\n",dbName.c_str(), sqlite3_errmsg(db));
	return true;
}

bool SqLight::Close(){
	int rc = sqlite3_close(db);
	if( rc ) {
	  posDebug( "Can't close database: %s\n", sqlite3_errmsg(db));
	} else {
	  //posDebug( "closed database successfully: %s\n", sqlite3_errmsg(db));
	  posDebug( "closed database successfully:\n");
	}
	return (rc?false:true);
}

int SqLight::callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	posDebug("SqLight::callback numcolls: %d\n",argc);
	ResultSet* results=(ResultSet*)data;
	Row *row = new Row;

	for(i = 0; i<argc; i++) {
		std::stringstream col;
		std::stringstream val;
		col<<azColName[i];
		//col<<i<<"#"<<azColName[i];
		val<<argv[i];
		posDebug("%s=%s:%lu\n", col.str().c_str(), val.str().c_str(), val.str().size() );
		(*row)[col.str()]=(val.str().size()) ? val.str() : "NULL";
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

