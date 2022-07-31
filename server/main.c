#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "types.h"

#include "connections_queue.h"
#include "http_parser.h"
#include "task_executors.h"

#define SERVER_PORT    "8080"
#define BUF_SIZE       4096
#define SOCKET_ERROR   (-1)
#define SERVER_BACKLOG 20
#define THREADS_POOL   20

typedef struct queue_handler_object {
    connections_queue* connections;
    pthread_mutex_t    mutex;
    pthread_cond_t     conditional;

    http_method_executors* executors;
} queue_handler_object;

int handle_connection(socket_descriptor client_socket, http_method_executors* executors);
int check(int exp, const char* msg);
char* get_content_type(const char* path);
void foo(void* arg);

void foo(void* arg) {
    char* s = (char*)arg;
    printf("%s\n gdkgnkdgkdjgjkdkgj", s);
}

void spam(socket_descriptor client_socket) {
    char response[2048];

    char buffer[BUF_SIZE];
    int bytes_received = recv(
        client_socket, buffer, BUF_SIZE, 0
    );
    printf("\r\n%s\r\n", buffer);
    
    sprintf(response, "%s", "HTTP/1.1 200 OK\r\n");
    sprintf(response + strlen(response), "%s", "Connection: close\r\n");
    sprintf(response + strlen(response), "%s", "Content-Type: text/event-stream\r\n\r\n");
    sprintf(response + strlen(response), "%s", "data: wukong comand\n\n");

    //printf("\n%s\n", response);   

    int bytes_sent = send(
        client_socket, response, strlen(response), 0
    );

    close(client_socket);

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
        
        handle_connection(client_s, executors);
    }
}

int main(int argc, char **argv) {
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
    check(server_socket, "Failed to create socket");

    int is_server_binded = bind(server_socket,
        bind_addres->ai_addr, bind_addres->ai_addrlen);
    check(is_server_binded, "Bind Failed!");    
    freeaddrinfo(bind_addres);

    int is_listen = listen(server_socket, SERVER_BACKLOG);
    check(is_listen, "Listen Failed!");

    http_method_executors* executors = init_executors(HTTP_METHODS_COUNT);
    add_executor(executors, handle_GET_html, "GET", "/");
    add_executor(executors, handle_GET_scripts, "GET", "/index.bundle.js");
    // add_executor(executors, foo, "GET", "/");
    // add_executor(executors, foo, "GET", "/");
    // add_executor(executors, foo, "GET", "/");

    connections_queue* connections = init_connections_queue();
    queue_handler_object arg_obj = {
        .connections = connections,
        .executors   = executors,
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

int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

int handle_connection(socket_descriptor client_socket, http_method_executors* executors) {
    char buffer[BUF_SIZE];
    int bytes_received = recv(
        client_socket, buffer, BUF_SIZE, 0
    );
    printf("\r\n%s\r\n", buffer);

    char method[100];
    parse_http_method(buffer, method);
    char uri[100];
    parse_http_uri(buffer, uri);
    
    void (*executor)(void*) = get_executor(executors, method, uri);

    task_args args = {
        .client_socket = client_socket
    };
    strcpy(args.url, uri);
    if (executor) {
        executor((void*)&args);
    }   

    return 0;
}

char* get_content_type(const char* path) {
    const char* last_dot = strrchr(path, '.');
    if (last_dot) {
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
    } 
    return "application/octet-stream";
}



    // char response[2048];
    
    // sprintf(response, "%s", "HTTP/1.1 200 OK\r\n");
    // sprintf(response + strlen(response), "%s", "Connection: close\r\n");
    // sprintf(response + strlen(response), "%s", "Content-Type: application/json\r\n\r\n");
    // sprintf(response + strlen(response), "%s", "{ \"name\" : \"Pedro\", \"age\" : \"22\" }");

    //printf("\n%s\n", response);   

    // int bytes_sent = send(
    //     client_socket, response, strlen(response), 0
    // );