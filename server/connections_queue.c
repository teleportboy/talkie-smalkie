#include <stdlib.h>
#include <stdio.h>

#include "connections_queue.h"
#include "types.h"

void enqueue_connection(connections_queue* connections, 
                        socket_descriptor client_socket) {
    queue_node* new_connection    = (queue_node*)malloc(sizeof(queue_node));
    new_connection->client_socket = client_socket;
    new_connection->next_node     = NULL;

    if (!connections->front) {
        connections->front = new_connection;
        connections->rear  = new_connection;
        return;
    }

    connections->rear->next_node = new_connection;
    connections->rear            = new_connection; 
}

socket_descriptor dequeue_connection(connections_queue* connections) {
    if (!connections->front) {
        return -1;
    }

    socket_descriptor client_socket = connections->front->client_socket;

    queue_node* old_front = connections->front;
    connections->front    = old_front->next_node;

    free(old_front);
    old_front = NULL;

    return client_socket;
}

connections_queue* init_connections_queue() {
    connections_queue* connections = 
        (connections_queue*)malloc(sizeof(connections_queue));
    connections->front = NULL;
    connections->rear  = NULL;

    return connections;
}

void display_queue(connections_queue* connections) {
    queue_node* queue_iterator = connections->front;
    while (queue_iterator) {
        printf("%d ", queue_iterator->client_socket);
        queue_iterator = queue_iterator->next_node;
    }
    printf("\n");
}