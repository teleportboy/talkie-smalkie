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

//to do: memmory leak
char* parse_http_url(char* http, char** url_query) {    
    int start = nth_char_index_of(http, ' ', 1) + 1;
    int end   = nth_char_index_of(http, ' ', 2);
    
    char* buffer = calloc(512, sizeof(char));
    memcpy(buffer, &http[start], end - start);

    char* path = strdup(strsep(&buffer, "?"));
    char* query = strsep(&buffer, "?");
    if (query) {
        *url_query = strdup(query);
    }

    free(buffer);

    return path;
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

char** parse_url_query(const char* src, const char* delim) {
    char** result = calloc(256, sizeof(char*));
    for (int i = 0; i < 256; i++) {
        result[i] = calloc(128, sizeof(char));
    }
    
    char* working = calloc(128, sizeof(char));
    strcpy(working, src);

    char* p = strtok(working, delim);
    for(int i = 0; p != NULL && i < (128 - 1); i++, p = strtok(NULL, delim)) {
        result[i] = p;
        result[i + 1] = NULL;
    }

    free(working);

    return result;
}

//to do: alloc memory from heap
void parse_json_body(char* json, char* dest, char* property) {
    char* uname_p = strstr(json, property);
    char* start   = uname_p + strlen(property) + 2;
    while (*(++start) != '\"') {
        *dest++ = *start;
    }    
}

void http_set_content_length(http* http, int size) {
    char* length = calloc(5, sizeof(char));
    sprintf(length, "%d", size);
    http->content_length = length;    
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

void http_ok_response(socket_descriptor client_socket, char* result) {
    http response;
    http_set_status_code(&response, "200 OK");
    http_set_connection_status(&response, "close");
    http_set_content_type(&response, "application/json");
    char* body = calloc(128, sizeof(char));
    sprintf(body, "{\"isOk\":\"%s\"}", result);
    http_set_content_length(&response, strlen(body));
    http_set_body(&response, body);

    http_response(&response, client_socket);

    free(body);
}

void http_json_response(socket_descriptor client_socket, char* json) {
    http response;
    http_set_status_code(&response, "200 OK");
    http_set_connection_status(&response, "close");
    http_set_content_type(&response, "application/json");
    char* body = calloc(4096, sizeof(char));
    sprintf(body, "%s", json);
    http_set_content_length(&response, strlen(body));
    http_set_body(&response, body);

    http_response(&response, client_socket);

    free(body);
}

void http_bad_response(socket_descriptor client_socket) {
    http response;
    http_set_status_code(&response, "400 Bad Request");
    http_set_connection_status(&response, "close");
    http_set_content_type(&response, "application/json");
    char* body = "{\"is_ok\":\"false\"}";
    http_set_content_length(&response, strlen(body));
    http_set_body(&response, body);

    http_response(&response, client_socket);
}

void http_response(http* response, socket_descriptor client_socket) {
    char* buffer = calloc(4096, sizeof(char));

    int length = 0;
    length  = sprintf(buffer,          "HTTP/1.1 %s\r\n", response->status_code);
    length += sprintf(buffer + length, "Connection: %s\r\n", response->connection_status);
    length += sprintf(buffer + length, "Content-Length: %s\r\n", response->content_length);
    length += sprintf(buffer + length, "Content-Type: %s\r\n\r\n", response->content_type);
    length += sprintf(buffer + length, "%s", response->body);
    

    int bytes_sent = 0;
    while (bytes_sent < length) {
        bytes_sent = send(client_socket, buffer, length, 0);
        if (bytes_sent == -1) {
            printf("oops\n");
            printf("response length %d bytes sent %d\n", strlen(buffer), bytes_sent);
            return;
        }
    }

    free(buffer);
}



//     sprintf(response, "%s", "HTTP/1.1 200 OK\r\n");
//     sprintf(response + strlen(response), "%s", "Connection: close\r\n");
//     sprintf(response + strlen(response), "%s", "Content-Type: text/event-stream\r\n\r\n");
//     sprintf(response + strlen(response), "%s", "data: wukong comand\n\n");