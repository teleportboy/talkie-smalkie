#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "types.h"

#include "database_api/database.h"
#include "connections_queue.h"
#include "http_parser.h"
#include "task_executors.h"
#include "amqp_api/rabbitmq.h"

#include "sse_api/sse.h"

#define SERVER_PORT    "8888"
#define BUF_SIZE       4096
#define SOCKET_ERROR   (-1)
#define SERVER_BACKLOG 20
#define THREADS_POOL   1

int handle_connection(socket_descriptor client_socket, 
    http_method_executors* executors, task_args* external_args);
int check(int exp, const char* msg);
void* thread_handler_function(void* arg);
socket_descriptor server_listen();

int main(int argc, char **argv) {
    data_base db;
    db_init(&db, "database.db");
    db_create_table(&db, "users", "uname, psw");
    db_create_table(&db, "chatrooms", "chatroom");
    //set_debug();

    socket_descriptor server_socket = server_listen();

    amqp_connection_state_t rabbitmq_connection = 
        rabbitmq_open_connection("localhost", 5672);

    http_method_executors* executors = init_executors(HTTP_METHODS_COUNT);
    add_executor(executors, GET_html,       "GET",  "/");
    add_executor(executors, GET_html,       "GET",  "/login");
    add_executor(executors, GET_html,       "GET",  "/registration");
    add_executor(executors, GET_favicon,    "GET",  "/favicon.ico");
    add_executor(executors, GET_scripts,    "GET",  "/index.bundle.js");
    add_executor(executors, POST_login,     "POST", "/login");
    add_executor(executors, POST_registr,   "POST", "/registration");
    add_executor(executors, POST_message,   "POST", "/message");
    add_executor(executors, POST_chatroom,  "POST", "/chatroom");
    add_executor(executors, GET_find_rooms, "GET",  "/search_chatrooms");
    add_executor(executors, POST_room_join, "POST", "/room_join");
    add_executor(executors, GET_chatrooms,  "GET",  "/chatrooms");
    add_executor(executors, GET_all_chats,  "GET",  "/all_chats");

    connections_queue* connections = init_connections_queue();

    task_args external_objs = {
        .data_base     = &db,
        .rabbitmq_conn = rabbitmq_connection
    };
    queue_handler_object arg_obj = {
        .external_objects = &external_objs,

        .executors   = executors,

        .connections = connections,        

        .mutex       = PTHREAD_MUTEX_INITIALIZER,
        .conditional = PTHREAD_COND_INITIALIZER
    };

    pthread_t threads_pool[THREADS_POOL];
    for (int i = 0; i < THREADS_POOL; i++) {
        pthread_create(&threads_pool[i], NULL, 
            thread_handler_function, (void*)&arg_obj);
    }
    
    while (1) {        
        socklen_t client_size = sizeof(sockaddr_in);
        sockaddr  client_address;

        socket_descriptor client_socket = accept( 
            server_socket, &client_address, &client_size
        );        
        check(client_socket, "accept failed");

        pthread_mutex_lock(&arg_obj.mutex);
        enqueue_connection(connections, client_socket);
        pthread_cond_signal(&arg_obj.conditional);
        pthread_mutex_unlock(&arg_obj.mutex);
    }

    close(server_socket);

    return 0;
}

void* thread_handler_function(void* arg) {
    queue_handler_object*  arg_obj     = ((queue_handler_object*)arg);
    connections_queue*     connections = arg_obj->connections;
    pthread_mutex_t*       mutex       = &arg_obj->mutex;
    pthread_cond_t*        conditional = &arg_obj->conditional;

    http_method_executors* executors   = arg_obj->executors;  

    socket_descriptor      client_s    = -1;  

    while (1) {
        pthread_mutex_lock(mutex);

        client_s = dequeue_connection(connections);
        if (client_s == SOCKET_ERROR) {
            pthread_cond_wait(conditional, mutex);
            client_s = dequeue_connection(connections);
        }
        
        pthread_mutex_unlock(mutex);
        
        handle_connection(client_s, executors, arg_obj->external_objects);
    }
}

int handle_connection(socket_descriptor client_socket, 
    http_method_executors* executors, task_args* external_objects) {

    char* buffer = calloc(2048, sizeof(char));
    int received = recv(client_socket, buffer, BUF_SIZE, 0);    

    char* method = calloc(100, sizeof(char));
    parse_http_method(buffer, method);

    char* url_query = NULL;
    char* url = parse_http_url(buffer, &url_query);
    
    printf("\nmethod: %s\nurl: %s query: %s\n", method, url, url_query);

    //Получить нужный колбэк учитывая http метод и путь 
    void (*executor)(void*) = get_executor(executors, method, url);
    if (!executor) {
        return 0;
    }

    //Аргументы для колбека. Это "внешние" объекты - БД, хэштаблица онлайна.
    //Это сокет клиента, http 
    task_args args = {
        .client_socket = client_socket,
        .rabbitmq_conn = external_objects->rabbitmq_conn,
        .data_base     = external_objects->data_base,
        .url           = strdup(url),
        .url_query     = url_query,
        .http          = strdup(buffer)
    };

    //Вызов обработчика 
    executor((void*)&args);

    free(buffer);
    free(method);
    free(url);
    free(args.url);
    free(args.http);
    //free(args.url_query);

    return 1;
}

socket_descriptor server_listen() {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
        
    addrinfo* bind_addres = &hints;
    getaddrinfo(NULL, SERVER_PORT, &hints, &bind_addres);

    socket_descriptor server_socket = socket(
        bind_addres->ai_family, bind_addres->ai_socktype,
        bind_addres->ai_protocol
    );
    check(server_socket, "Failed to create socket\n");

    int yes = 1;
    int is_reusable = setsockopt(server_socket, SOL_SOCKET, 
        SO_REUSEADDR, &yes, sizeof(int));
    check(is_reusable, "Reuse Failed!\n");

    int is_server_binded = bind(server_socket,
        bind_addres->ai_addr, bind_addres->ai_addrlen);
    check(is_server_binded, "Bind Failed!\n"); 

    freeaddrinfo(bind_addres);

    int is_listen = listen(server_socket, SERVER_BACKLOG);
    check(is_listen, "Listen Failed!\n");

    return server_socket;
}

int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}