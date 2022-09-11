#ifndef CONNECTIONS_QUEUE_INCLUDED
#define CONNECTIONS_QUEUE_INCLUDED

#include "../types.h"

connections_queue* init_connections_queue();
void enqueue_connection(connections_queue*, socket_descriptor);
socket_descriptor dequeue_connection(connections_queue* connections);
void display_queue(connections_queue* connections);

#endif