#include "string.h"

#include "stddef.h"

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) p++;
    return p - s;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

char *strtok(char *str, const char *delim) {
    static char *last = 0;
    if (str) {
        last = str;
    }
    if (!last) return 0;

    char *start = last;
    while (*start) {
        const char *d = delim;
        int is_delim = 0;
        while (*d) {
            if (*start == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (!is_delim) break;
        start++;
    }
    if (!*start) {
        last = 0;
        return 0;
    }

    char *end = start;
    while (*end) {
        const char *d = delim;
        while (*d) {
            if (*end == *d) {
                *end = '\0';
                last = end + 1;
                return start;
            }
            d++;
        }
        end++;
    }

    last = 0;
    return start;
}