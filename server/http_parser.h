#ifndef HTTP_PARSER_INCLUDED
#define HTTP_PARSER_INCLUDED

void parse_http_method(char* http, char* dest);
void parse_http_uri(char* http, char* dest);

#endif