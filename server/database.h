#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include "./sqlite3_api/sqlite3.h"

typedef struct data_base {
	sqlite3*      data_base;
	sqlite3_stmt* statement;
} data_base;

int  db_init(data_base* db, char* filename);
void db_close(data_base* db);
int  db_create_table(data_base* db, char* table, char* columns);
int  db_drop_table(data_base* db, char* table);
int  db_add_record(data_base* db, char* table, char* values);

#endif