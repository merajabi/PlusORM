#include <stdio.h>
#include <sqlite3.h> 

int main(int argc, char* argv[]) {
   sqlite3 *db;
   //char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("sqlite.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully: %s\n", sqlite3_errmsg(db));
   }

   rc = sqlite3_close(db);
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "Closed database successfully:\n");
   }

   return(0);
}
