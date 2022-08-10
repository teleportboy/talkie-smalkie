#include <stdio.h>

#include "database.h"

int db_init(data_base* db, char* filename) {
	if (sqlite3_open(filename, &db->data_base)) {
        printf("oshibka init\n");		
		return 0;
	}
	return 1;
}

void db_close(data_base* db) {
    sqlite3_close(db->data_base);
}

//todo нормально колонки
int db_create_table(data_base* db, char* table, char* columns) {	
    char query[128];
	sprintf(query, "create table if not exists %s(%s)", table, columns);

	int res = 0;
	if ((res = sqlite3_exec(db->data_base, query, 0, 0, NULL))) { 
        printf("result of creation table is: %d\n", res);
		return 0;
	}
	return 1;
}

int db_drop_table(data_base* db, char* table) {
	char query[128];
	sprintf(query, "DROP TABLE IF EXISTS %s", table);

	if (sqlite3_exec(db->data_base, query, 0, 0, NULL)) {
        printf("oshibochka drop tablle\n");
		return 0;
	}
	return 1;
}

int db_add_record(data_base* db, char* table, char* values) {
	char query[256];
	sprintf(query, "INSERT INTO %s VALUES(%s)", table, values);

    if (sqlite3_exec(db->data_base, query, 0, 0, NULL)) {
        printf("oshibochka add record");       
        return 0;
    }

	return 1;
}