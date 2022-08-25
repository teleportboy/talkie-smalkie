#ifndef RABBITMQ_DEFINED_H
#define RABBITMQ_DEFINED_H

#include <rabbitmq-c/amqp.h>

amqp_connection_state_t rabbitmq_open_connection(char* hostname, int port);
void rabbitmq_close_connection(amqp_connection_state_t conn);
void rabbitmq_send_message(amqp_connection_state_t conn, char* queue,
                          char* exchange, char* message);
char* rabbitmq_get_message(amqp_connection_state_t conn, char* queue_name);
void send_batch(amqp_connection_state_t conn, char* queue_name,
                       char* exchange, char* message);

#endif