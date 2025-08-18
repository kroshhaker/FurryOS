#include "string.h"

#include "stddef.h"
#include "stdint.h"

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

char *strcpy(char *dst, const char *src) {
    size_t i = 0;
    while ((dst[i] = src[i])) i++;
    return dst;
}

void *memset(void *ptr, int c, size_t n) {
    uint8_t *p = ptr;
    for (size_t i = 0; i < n; i++)
        p[i] = (uint8_t)c;
    return ptr;
}

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = dst;
    const uint8_t *s = src;
    for (size_t i = 0; i < n; i++)
        d[i] = s[i];
    return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
    uint8_t *d = dst;
    const uint8_t *s = src;
    if (d < s) {
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    } else {
        for (size_t i = n; i > 0; i--)
            d[i-1] = s[i-1];
    }
    return dst;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *a = s1, *b = s2;
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i])
            return a[i] - b[i];
    }
    return 0;
}