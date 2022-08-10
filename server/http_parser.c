#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "http_parser.h"
#include "utility.h"
#include "types.h"

void parse_http_method(char* http, char* dest) {
    int index = nth_char_index_of(http, ' ', 1);    
    memcpy(dest, http, index);    
}

void parse_http_uri(char* http, char* dest) {
    int start = nth_char_index_of(http, ' ', 1) + 1;
    int end   = nth_char_index_of(http, ' ', 2);
    
    memcpy(dest, &http[start], end - start);

    printf("dest: %s\n", dest);
}

int parse_http_content_length(char* http) {
    const char* cl = "Content-Length: ";

    char* cl_p  = strstr(http, cl);
    int offset  = strlen(cl);
    char* start = cl_p + offset;
    char* end   = strchr(start, '\r');

    char cl_buffer[10];
    memcpy(cl_buffer, start, end - start);

    int content_length = atoi(cl_buffer);

    return content_length;
}

void parse_http_body(char* http, char* dest) {
    int content_length = parse_http_content_length(http);
    char* before_body  = strrchr(http, '\n') + 1;
    memcpy(dest, before_body, content_length);
}

void parse_json_body(char* json, char* dest, char* property) {
    char* uname_p = strstr(json, property);
    char* start   = uname_p + strlen(property) + 2;
    while (*(++start) != '\"') {
        *dest++ = *start;
    }    
}

void http_set_content_length(http* http, char* str) {
    http->content_length = strdup(str);    
}

void http_set_status_code(http* http, char* str) {
    http->status_code = strdup(str);    
}

void http_set_content_type(http* http, char* str) {
    http->content_type = strdup(str);    
} 
 
void http_set_method(http* http, char* str) {
    http->method = strdup(str);    
}

void http_set_body(http* http, char* str) {
    http->body = strdup(str);    
}

void http_set_url(http* http, char* str) {
    http->url = strdup(str);    
}

void http_set_connection_status(http* http, char* str) {
    http->connection_status = strdup(str);
}

void http_response(http* response, socket_descriptor client_socket) {
    char* buffer = calloc(4096, sizeof(char));

    int length = 0;
    length  = sprintf(buffer,          "HTTP/1.1 %s\r\n", response->status_code);
    length += sprintf(buffer + length, "Connection: %s\r\n", response->connection_status);
    length += sprintf(buffer + length, "Content-Type: %s\r\n\r\n", response->content_type);
    length += sprintf(buffer + length, "%s", response->body);
    
    int bytes_sent = 0;
    while (bytes_sent < length) {
        bytes_sent = send(client_socket, buffer, length, 0);
        if (bytes_sent == -1) {
            printf("eee kuda ti lezish\n");
            return;
        }
    }
}

//     sprintf(response, "%s", "HTTP/1.1 200 OK\r\n");
//     sprintf(response + strlen(response), "%s", "Connection: close\r\n");
//     sprintf(response + strlen(response), "%s", "Content-Type: text/event-stream\r\n\r\n");
//     sprintf(response + strlen(response), "%s", "data: wukong comand\n\n");