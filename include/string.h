#ifndef _STRING_H
#define _STRING_H

#include "stddef.h"

extern size_t strlen(const char *s);
extern int strcmp(const char* str1, const char* str2);
extern char *strtok(char *str, const char *delim);

#endif