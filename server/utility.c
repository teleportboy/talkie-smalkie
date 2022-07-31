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