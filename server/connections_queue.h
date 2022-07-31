#ifndef CONNECTIONS_QUEUE_INCLUDED
#define CONNECTIONS_QUEUE_INCLUDED

#include "types.h"

typedef struct queue_node {
    socket_descriptor  client_socket;    
    struct queue_node* next_node;
} queue_node;

typedef struct connections_queue {
    queue_node* front;
    queue_node* rear;
} connections_queue;

connections_queue* init_connections_queue();
void enqueue_connection(connections_queue*, socket_descriptor);
socket_descriptor dequeue_connection(connections_queue* connections);
void display_queue(connections_queue* connections);

#endif