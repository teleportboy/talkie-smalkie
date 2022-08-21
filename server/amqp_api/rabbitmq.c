#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "utils.h"
#include "rabbitmq.h"

int rabbitmq_send_message(char* queue, char* exchange, char* message) {
    amqp_connection_state_t conn;
    conn = amqp_new_connection();

    amqp_socket_t *socket = NULL;
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    char const *hostname = "localhost";
    const int port = 5672;
    int status = amqp_socket_open(socket, hostname, port);
    if (status) {
        die("opening TCP socket");
    }

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                                "guest", "guest"),
                        "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    send_batch(conn, queue, exchange, message);

    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                        "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                        "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");
    
    return 0;
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
