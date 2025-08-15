#ifndef _CONVERT_H
#define _CONVERT_H

#include "stddef.h"

extern unsigned long hex_to_dec(const char *hex);
extern unsigned long hex_to_dec_len(const char *hex, size_t len);
extern char *i2s(int num, char *str);
extern int s2i(const char *s);

#endif