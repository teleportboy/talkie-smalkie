#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "database.h"
#include "../utility_features/json_parser.h"

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
    char* query = calloc(128, sizeof(char));
	sprintf(query, "create table if not exists %s(%s)", table, columns);

	int res = 0;
	if ((res = sqlite3_exec(db->data_base, query, 0, 0, NULL))) { 
        printf("result of creation table is: %d\n", res);
		free(query);
		return 0;
	}
	free(query);
	return 1;
}

int db_is_exist(data_base* db, char* table, char* column, const char* value) {
	char* query = calloc(128, sizeof(char));
	sprintf(query, "SELECT * FROM users WHERE %s = '%s'", column, value);

    sqlite3_stmt* selectstmt;
    int result = sqlite3_prepare_v2(db->data_base, query, -1, &selectstmt, NULL);
	printf("%d\n", result);
    if (result == SQLITE_OK) {
		if (sqlite3_step(selectstmt) == SQLITE_ROW) {
			printf("find\n");
			sqlite3_finalize(selectstmt);
			free(query);
			return 1;		
		}		
    }
    sqlite3_finalize(selectstmt);
	free(query);
	return 0;
}

int db_is_table_exist(data_base* db, char* table) {
	char* query = calloc(128, sizeof(char));
	sprintf(query,
			"SELECT name FROM sqlite_master WHERE type='table' AND name='%s';",
			table
	);
	sqlite3_stmt* selectstmt;
    int result = sqlite3_prepare_v2(db->data_base, query, -1, &selectstmt, NULL);
    if (result == SQLITE_OK) {
		if (sqlite3_step(selectstmt) == SQLITE_ROW) {
			printf("find\n");
			sqlite3_finalize(selectstmt);
			free(query);
			return 1;		
		}		
    }
    sqlite3_finalize(selectstmt);
	free(query);
	return 0;
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

    int res = sqlite3_exec(db->data_base, query, 0, 0, NULL);
    printf("add record result: %d\n", res);

	return 1;
}

char** db_select_matches(data_base* db, char* table, char* match, int* count) {
	char* query = calloc(128, sizeof(char));
	sprintf(query, "SELECT * FROM %s WHERE chatroom LIKE '%%%s%%';", table, match);

	sqlite3_stmt* selectstmt;
	char** matches = calloc(*count, sizeof(char*));
    int result = sqlite3_prepare_v2(db->data_base, query, -1, &selectstmt, NULL);
	if (result == SQLITE_OK) {
		int i = 0;
		while (1) {
			int rc = sqlite3_step(selectstmt);
			if (rc == SQLITE_DONE) break;
			if (rc != SQLITE_ROW) break;
			if (i == *count) break;

			matches[i++] = strdup(sqlite3_column_text(selectstmt, 0));				
		}
		*count = i;
	}
	
	free(query);
	sqlite3_finalize(selectstmt);

	return matches;
}

json_object* db_select_columns_as_json(data_base* db, char* table, int columns_count, int* count) {
	char* query = calloc(128, sizeof(char));
	sprintf(query, "SELECT * FROM %s", table);

	json_object* json = json_object_new_array_ext(256);

	sqlite3_stmt* selectstmt;
	int result = sqlite3_prepare_v2(db->data_base, query, -1, &selectstmt, NULL);
	if (result == SQLITE_OK) {
		int j = 0;
		while (1) {
			int rc = sqlite3_step(selectstmt);
			if (rc == SQLITE_DONE) break;
			if (rc != SQLITE_ROW) break;
			if (++j == *count) break;

			json_object* jobj = json_object_new_object();
			json_set_value(jobj, "message", (char*)sqlite3_column_text(selectstmt, 0));
			json_set_value(jobj, "sender", (char*)sqlite3_column_text(selectstmt, 1));
			json_object_array_add(json, jobj);			
		}
		*count = j;	
	}

	return json;

}

char** db_select_column(data_base* db, char* table, char* column, int* count) {
	char* query = calloc(128, sizeof(char));
	sprintf(query, "SELECT %s FROM %s", column, table);

	sqlite3_stmt* selectstmt;
	char** selects = calloc(*count, sizeof(char*));
	int result = sqlite3_prepare_v2(db->data_base, query, -1, &selectstmt, NULL);
	if (result == SQLITE_OK) {
		int i = 0;
		while (1) {
			int rc = sqlite3_step(selectstmt);
			if (rc == SQLITE_DONE) break;
			if (rc != SQLITE_ROW) break;
			if (i == *count) break;

			selects[i++] = strdup(sqlite3_column_text(selectstmt, 0));				
		}
		*count = i;
	}

	free(query);
	sqlite3_finalize(selectstmt);

	return selects;
}