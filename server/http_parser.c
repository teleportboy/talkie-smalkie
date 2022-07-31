#include <string.h>
#include <stdio.h>
#include "utility.h"

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