#ifndef TASK_EXECUTORS_H
#define TASK_EXECUTORS_H

#define HTTP_METHODS_COUNT 4

#include "types.h"

http_method_executors* init_executors(int count);

void* get_executor(http_method_executors* executors, char* method, char* url);
void add_executor(http_method_executors* executors, void (*p_executor)(void*), 
                  char* method, char* url);

int get_http_method_id(char* method);
int serve_file(const socket_descriptor client_socket, const char* path, const char* ct);

void GET_html(void* data);
void GET_scripts(void* data);
void GET_favicon(void* data);

void POST_login(void* data);
void POST_registr(void* data);

void POST_message(void* data);
void GET_message(void* data);
void POST_chatroom(void* data);
void GET_find_rooms(void* data);
void GET_chatrooms(void* data);
void GET_all_chats(void* data);
void POST_room_join(void* data);

#endif