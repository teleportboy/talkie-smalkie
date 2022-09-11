#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include "sqlite3.h"
#include <json-c/json.h>

typedef struct data_base {
	sqlite3*      data_base;
	sqlite3_stmt* statement;
} data_base;

int    db_init(data_base* db, char* filename);
void   db_close(data_base* db);
int    db_create_table(data_base* db, char* table, char* columns);
int    db_drop_table(data_base* db, char* table);
int    db_add_record(data_base* db, char* table, char* values);
int    db_is_exist(data_base* db, char* table, char* column, const char* value);
int    db_is_table_exist(data_base* db, char* table);
char** db_select_matches(data_base* db, char* table, char* match, int* count);
char** db_select_column(data_base* db, char* table, char* column, int* count);

json_object* db_select_columns_as_json(data_base* db, char* table, int columns_count, int* count);

#endif