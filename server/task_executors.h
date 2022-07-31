#ifndef TASK_EXECUTORS_H
#define TASK_EXECUTORS_H

#define HTTP_METHODS_COUNT 4

#include "types.h"

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
    socket_descriptor client_socket;
    char url[256];
} task_args;

http_method_executors* init_executors(int count);

void* get_executor(http_method_executors* executors, char* method, char* url);
void add_executor(http_method_executors* executors, void (*p_executor)(void*), 
                  char* method, char* url);

int get_http_method_id(char* method);
int serve_file(const socket_descriptor client_socket, const char* path, const char* ct);

void handle_GET_html(void* data);
void handle_GET_scripts(void* data);

#endif