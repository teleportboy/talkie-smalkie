#ifndef HTTP_PARSER_H_INCLUDED
#define HTTP_PARSER_H_INCLUDED

#include "types.h"

typedef struct http {
    char* status_code;
    char* method;
    char* url;
    char* content_type;
    char* content_length;
    char* body;
    char* connection_status;
} http;

void parse_http_method(char* http, char* dest);
void parse_http_url(char* http, char* dest);
void parse_http_body(char* http, char* dest);

void parse_json_body(char* json, char* dest, char* property);

void http_set_connection_status(http* http, char* str);
void http_set_content_length(http* http, char* str);
void http_set_content_type(http* http, char* str); 
void http_set_status_code(http* http, char* str);
void http_set_method(http* http, char* str);
void http_set_body(http* http, char* str);
void http_set_url(http* http, char* str);
//to do
void http_free(http* http);

void http_response(http* response, socket_descriptor client_socket);

#endif