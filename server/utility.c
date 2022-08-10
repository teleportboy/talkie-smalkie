#include <stdlib.h>
#include <string.h>

int nth_char_index_of(char* str, char el, int nth) {
    int current_n = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == el) {
            current_n++;
        }
        if (current_n == nth) {
            return i;
        }
    }
    return -1;    
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    
    return result;
}