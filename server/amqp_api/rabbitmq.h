#ifndef RABBITMQ_DEFINED_H
#define RABBITMQ_DEFINED_H

#include <rabbitmq-c/amqp.h>

int rabbitmq_send_message(char* queue, char* exchange, char* message);
void send_batch(amqp_connection_state_t conn, char* queue_name,
                       char* exchange, char* message);

#endif