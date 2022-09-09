#ifndef TYPES_INCLUDED_H
#define TYPES_INCLUDED_H

#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <rabbitmq-c/amqp.h>

#include "database_api/database.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr    sockaddr;
typedef struct addrinfo    addrinfo;

typedef int socket_descriptor;

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
    char*             url;
    char*             http;
    char*             url_query;
    
    socket_descriptor client_socket;
    
    data_base*              data_base;
    amqp_connection_state_t rabbitmq_conn;
} task_args;

typedef struct queue_node {
    socket_descriptor  client_socket;    
    struct queue_node* next_node;
} queue_node;

typedef struct connections_queue {
    queue_node* front;
    queue_node* rear;
} connections_queue;

typedef struct queue_handler_object {
    connections_queue* connections;
    pthread_mutex_t    mutex;
    pthread_cond_t     conditional;

    http_method_executors* executors;
    task_args*             external_objects;
} queue_handler_object;

#endif