#ifndef _STRING_H
#define _STRING_H

#include "stddef.h"

extern size_t strlen(const char *s);
extern int strcmp(const char* str1, const char* str2);
extern char *strtok(char *str, const char *delim);
extern char *strcpy(char *dst, const char *src);
extern void *memset(void *ptr, int c, size_t n);
extern void *memmove(void *dst, const void *src, size_t n);
extern void *memcpy(void *dst, const void *src, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);

#endif