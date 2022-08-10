#ifndef TASK_EXECUTORS_H
#define TASK_EXECUTORS_H

#define HTTP_METHODS_COUNT 4

#include "types.h"
#include "database.h"
#include "online_users_hashtable.h"

typedef struct executor {
    char  url[100];
    void  (*executor)(void*);
    int   is_exist;

    struct executor* next;
} executor;

typedef struct http_method_executors {
    int method_id;

    executor* list_head;
    executor* list_tail;
} http_method_executors;

typedef struct task_args {
    char              url[256];
    char              http[2048];
    data_base*        db;
    socket_descriptor client_socket;
    hash_table*       online_users;
} task_args;

http_method_executors* init_executors(int count);

void* get_executor(http_method_executors* executors, char* method, char* url);
void add_executor(http_method_executors* executors, void (*p_executor)(void*), 
                  char* method, char* url);

int get_http_method_id(char* method);
int serve_file(const socket_descriptor client_socket, const char* path, const char* ct);

void GET_html(void* data);
void GET_scripts(void* data);

void POST_login(void* data);
void POST_registr(void* data);
void POST_message(void* data);

#endif