#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task_executors.h"
#include "http_parser.h"
#include "database.h"

#include "types.h"

http_method_executors* init_executors(int count) {
    http_method_executors* executors = malloc (
                           sizeof(http_method_executors) * count
    );

    for (int i = 0; i < count; i++) {
        executors[i].method_id = i;
        executors[i].list_head = NULL;
        executors[i].list_tail = NULL;
    }

    return executors;
}

void add_executor(http_method_executors* executors, void (*p_executor)(void*), 
                  char* method, char* url) {
    executor* new_executor = malloc(sizeof(executor));

    strcpy(new_executor->url, url); 
    new_executor->executor  = p_executor;
    new_executor->next      = NULL;
    new_executor->is_exist  = 1;    

    int id = get_http_method_id(method);
    if (!executors[id].list_head) {
        executors[id].list_head = new_executor;
        executors[id].list_tail = new_executor;
        return;
    }

    executors[id].list_tail->next = new_executor;
    executors[id].list_tail       = new_executor;
}

void* get_executor(http_method_executors* executors, char* method, char* url) {
    int id = get_http_method_id(method);
    if (!executors[id].list_head) {
        return NULL;
    }

    executor* iterator = executors[id].list_head;

    while(iterator) {
        if (strcmp(iterator->url, url) == 0) {
            return iterator->executor;
        }
        iterator = iterator->next;
    }

    return NULL;
}

int get_http_method_id(char* method) {
    const char* methods[] = {
        "GET",
        "POST",
        "DELETE",
        "PUT"
    };

    for (int i = 0; i < HTTP_METHODS_COUNT; i++){
        if (strcmp(methods[i], method) == 0) {
            return i;
        }
    }

    return -1;
}

int serve_file(const socket_descriptor client_socket, const char* path, const char* ct) {
    FILE *fp = fopen(path, "rb");

    fseek(fp, 0L, SEEK_END);
    size_t cl = ftell(fp);
    rewind(fp);

    const int BSIZE = 1024;
    char buffer[BSIZE];

    sprintf(buffer, "HTTP/1.1 200 OK\r\n");
    send(client_socket, buffer, strlen(buffer), 0);

    sprintf(buffer, "Connection: close\r\n");
    send(client_socket, buffer, strlen(buffer), 0);

    sprintf(buffer, "Content-Length: %u\r\n", cl);
    send(client_socket, buffer, strlen(buffer), 0);

    sprintf(buffer, "Content-Type: %s\r\n", ct);
    send(client_socket, buffer, strlen(buffer), 0);
    printf("content type: %s\r\n\r\n", ct);

    sprintf(buffer, "\r\n");
    send(client_socket, buffer, strlen(buffer), 0);

    int r = fread(buffer, 1, BSIZE, fp);
    while (r) {
        send(client_socket, buffer, r, 0);
        r = fread(buffer, 1, BSIZE, fp);
    }

    fclose(fp);
}

void handle_GET_html(void* data) {
    task_args* args = (task_args*)data;
    serve_file(args->client_socket, "public/index.html", "text/html");
    close(args->client_socket);
}

void handle_GET_scripts(void* data) {
    task_args* args = (task_args*)data;
    serve_file(args->client_socket, "public/index.bundle.js", "application/javascript");
    close(args->client_socket);
}

void handle_POST_login(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = calloc(512, sizeof(char));
    parse_http_body(args->http, http_body);

    char* name_ = calloc(512, sizeof(char));
    parse_json_body(http_body, name_, "uname");
    char* psw_ = calloc(512, sizeof(char));
    parse_json_body(http_body, psw_, "psw");

    char* record_values = calloc(512, sizeof(char));
    sprintf(record_values, "'%s', '%s'", name_, psw_);
    printf("rec values %s\n", record_values);

    http response;
    http_set_status_code(&response, "200 OK");
    http_set_connection_status(&response, "close");
    http_set_content_type(&response, "application/json");
    http_set_body(&response, "{\"is_ok\":\"true\"}");

    http_response(&response, args->client_socket);

    free(http_body);
    free(name_);
    free(psw_);
    free(record_values);

    close(args->client_socket);
    //db_add_record(args->db, "users", record_values);
}