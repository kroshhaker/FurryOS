#ifndef	_CTYPE_H
#define	_CTYPE_H	1

#include "stdint.h"


static inline int isxdigit(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'F') ||
            (c >= 'a' && c <= 'f'));
}

#endif /* ctype.h  */
