#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "task_executors.h"
#include "../utility_features/http_parser.h"
#include "../utility_features/utility.h"
#include "../utility_features/json_parser.h"

#include "../sse/sse.h"

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

void GET_html(void* data) {
    task_args* args = (task_args*)data;
    serve_file(args->client_socket, "./public/index.html", "text/html");
    close(args->client_socket);
}

void GET_favicon(void* data) {
    task_args* args = (task_args*)data;
    serve_file(args->client_socket, "./public/favicon.ico", "image/x-icon");
    close(args->client_socket);
}

void GET_scripts(void* data) {
    task_args* args = (task_args*)data;
    serve_file(args->client_socket, "./public/index.bundle.js", "application/javascript");
    close(args->client_socket);
}

//to do: refactor, refactor and refactor
void POST_registr(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = malloc(512 * sizeof(char));
    parse_http_body(args->http, http_body);    
    
    json_object* json  = json_tokener_parse(http_body);
    char* uname = json_get_property(json, "uname");
    char* psw   = json_get_property(json, "psw");

    int is_exists = db_is_exist(args->data_base, "users", "uname", uname);
    if (!is_exists) {
        char* record_values = calloc(128, sizeof(char));
        sprintf(record_values, "'%s', '%s'", uname, psw);
        db_add_record(args->data_base, "users", record_values);

        //Создать таблицу чатов новоиспечённого пользователя
        db_create_table(args->data_base, uname, "chatroom");

        http_ok_response(args->client_socket, "true");
        free(record_values);
    } else {
        http_ok_response(args->client_socket, "false");
    }

    close(args->client_socket);

    free(http_body);
}

void POST_login(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = calloc(512, sizeof(char));
    parse_http_body(args->http, http_body);

    json_object* json = json_tokener_parse(http_body);
    free(http_body);

    char* uname = json_get_property(json, "uname");
    char* psw   = json_get_property(json, "psw");

    int is_correct[2];
    is_correct[0] = db_is_exist(args->data_base, "users", "uname", uname);
    is_correct[1] = db_is_exist(args->data_base, "users", "psw", psw);

    if (is_correct[0] && is_correct[1]) {
        http_ok_response(args->client_socket, "true");
        return;
    }
    
    http_ok_response(args->client_socket, "false");    
}

void POST_chatroom(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = calloc(512, sizeof(char));
    parse_http_body(args->http, http_body);

    json_object* json = json_tokener_parse(http_body);
    free(http_body);

    char* chatroom = json_get_property(json, "chatroom");
    char* room_record = calloc(128, sizeof(char));
    sprintf(room_record, "%s", chatroom);
    int is_exist = db_is_table_exist(args->data_base, room_record);

    if (!is_exist) {
        db_create_table(args->data_base, room_record, "message, sender"); 
        free(room_record);

        char* record_value = calloc(128, sizeof(char));
        sprintf(record_value, "'%s'", chatroom);
        db_add_record(args->data_base, "chatrooms", record_value);
        free(record_value);

        http_ok_response(args->client_socket, "true");
    }

    http_ok_response(args->client_socket, "false");
}

void GET_find_rooms(void* data) {
    task_args* args = (task_args*)data;

    char* match = strdup(strchr(args->url_query, '=') + 1);
    int chatrooms_count = 1000;
    char** chatrooms = db_select_matches(args->data_base, 
           "chatrooms", match, &chatrooms_count
    );

    json_object* jobj = json_object_new_object();
    json_object* chatrooms_json = json_object_new_array();
    for (int i = 0; i < chatrooms_count; i++) {
        json_set_array_property(&chatrooms_json, chatrooms[i]);
    }
    json_object_object_add(jobj, "chatrooms", chatrooms_json);

    printf("rooms searched: %s\n", (char*)json_object_to_json_string(jobj));
    http_json_response(args->client_socket, (char*)json_object_to_json_string(jobj));
    
    free(match);  
}

void POST_room_join(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = calloc(512, sizeof(char));
    parse_http_body(args->http, http_body);

    json_object* json = json_tokener_parse(http_body);
    free(http_body);

    char* uname    = json_get_property(json, "uname");
    char* chatroom = json_get_property(json, "chatroom");

    char* record_new_chat = calloc(128, sizeof(char));
    sprintf(record_new_chat, "'%s'", chatroom);

    db_add_record(args->data_base, uname, record_new_chat);
    free(record_new_chat);

    http_ok_response(args->client_socket, "true");
}

void GET_chatrooms(void* data) {
    task_args* args = (task_args*)data;

    char* match = strdup(strchr(args->url_query, '=') + 1);
    int chatrooms_count = 1000;
    char** chatrooms = db_select_column(args->data_base, 
           match, "chatroom", &chatrooms_count
    );

    json_object* jobj = json_object_new_object();
    json_object* chatrooms_json = json_object_new_array();
    for (int i = 0; i < chatrooms_count; i++) {
        json_set_array_property(&chatrooms_json, chatrooms[i]);
    }
    json_object_object_add(jobj, "chatrooms", chatrooms_json);

    printf("rooms searched: %s\n", (char*)json_object_to_json_string(jobj));
    http_json_response(args->client_socket, (char*)json_object_to_json_string(jobj));
    
    free(match);
    for (int i = 0; i < chatrooms_count; i++) {
        free(chatrooms[i]);
    }
    free(chatrooms);
}

void POST_message(void* data) {
    task_args* args = (task_args*)data;

    char* http_body = calloc(1024, sizeof(char));
    parse_http_body(args->http, http_body);

    json_object* json = json_tokener_parse(http_body);
    
    char* message  = json_get_property(json, "message");
    char* chatroom = json_get_property(json, "chatroom");
    char* sender   = json_get_property(json, "sender");

    json_object* data_to_send = json_object_new_object();
    json_set_value(data_to_send, "message", message);
    json_set_value(data_to_send, "chatroom", chatroom);
    json_set_value(data_to_send, "sender", sender);

    char* send = (char*)json_object_to_json_string(data_to_send);
    send_message(send, chatroom);

    char* record_values = calloc(512, sizeof(char));
    sprintf(record_values, "'%s', '%s'", message, sender);
    db_add_record(args->data_base, chatroom, record_values);

    http_ok_response(args->client_socket, "true");
    free(http_body);
}

void GET_all_chats(void* data) {
    task_args* args = (task_args*)data;

    char* match = strdup(strchr(args->url_query, '=') + 1);
    int chatrooms_count = 1000;
    char** chatrooms = db_select_column(args->data_base, 
           match, "chatroom", &chatrooms_count
    );

    int count = 1000;
    json_object* all_chats = json_object_new_object();
    json_object* chats = json_object_new_array();
    for (int i = 0; i < chatrooms_count; i++) {
        json_object* chat = json_object_new_object();
        json_set_value(chat, "chatroom", chatrooms[i]);
        json_object_object_add(chat, "messages",
            db_select_columns_as_json(args->data_base, chatrooms[i], 2, &count)
        );
        json_object_array_add(chats, chat);
    }
    json_object_object_add(all_chats, "allChats", chats);

    http_json_response(args->client_socket, (char*)json_object_to_json_string(all_chats));

    free(match);
    for (int i = 0; i < chatrooms_count; i++) {
        free(chatrooms[i]);
    }
    free(chatrooms);
}