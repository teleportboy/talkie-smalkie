#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "utils.h"
#include "rabbitmq.h"

amqp_connection_state_t rabbitmq_open_connection(char* hostname, int port) {
    amqp_connection_state_t conn = amqp_new_connection();

    amqp_socket_t* socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    int status = amqp_socket_open(socket, hostname, port);
    if (status) {
        die("opening TCP socket");
    }

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                                "guest", "guest"),
                        "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    return conn;
}

void rabbitmq_close_connection(amqp_connection_state_t conn) {
    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                        "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                        "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");
}

void rabbitmq_send_message(amqp_connection_state_t conn, char* queue,
                           char* exchange, char* message) {

    send_batch(conn, queue, exchange, message);
}

char* rabbitmq_get_message(amqp_connection_state_t conn, char* queue_name) {
    amqp_bytes_t queue = {
        .bytes = strdup(queue_name),
        .len = strlen(queue_name)
    };
    amqp_basic_consume(conn, 1, queue, amqp_empty_bytes, 0, 0, 0,
                        amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

    amqp_envelope_t envelope;
    amqp_rpc_reply_t ret = amqp_consume_message(conn, &envelope, NULL, 0);

    char* message = calloc(envelope.message.body.len + 1, sizeof(char));
    sprintf(message, "%s", envelope.message.body.bytes);

    amqp_basic_nack(conn, 1, envelope.delivery_tag, 0, 1);
    
    return message;
}

void send_batch(amqp_connection_state_t conn, char* queue_name,
                       char* exchange, char* message) {

    amqp_bytes_t message_bytes;
    message_bytes.len = strlen(message) * sizeof(char);
    message_bytes.bytes = (void*)message;

    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG |
                   AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2;

    die_on_error(amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange),
                                        amqp_cstring_bytes(queue_name), 0, 0, &props,
                                        message_bytes),
                        "Publishing");
  
}
