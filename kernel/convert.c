#include "convert.h"

#include "stdint.h"

unsigned long hex_to_dec(const char *hex) {
    unsigned long result = 0;
    char c;

    while ((c = *hex++)) {
        result <<= 4; // умножаем на 16
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        } else {
            // Ошибка: не hex-символ
            return 0;
        }
    }
    return result;
}

unsigned long hex_to_dec_len(const char *hex, size_t len) {
    unsigned long result = 0;
    for (size_t i = 0; i < len; i++) {
        char c = hex[i];
        result <<= 4;
        if (c >= '0' && c <= '9') result += c - '0';
        else if (c >= 'A' && c <= 'F') result += c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') result += c - 'a' + 10;
        else return 0; // ошибка
    }
    return result;
}

char *i2s(int num, char *str) {
    char temp[12];
    int i = 0, j = 0;
    int negative = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (num < 0) {
        negative = 1;
        num = -num;
    }

    while (num > 0) {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (negative)
        temp[i++] = '-';

    while (i > 0)
        str[j++] = temp[--i];

    str[j] = '\0';
    return str;
}

int s2i(const char *s) {
    int num = 0;
    int sign = 1;

    if (*s == '-') {
        sign = -1;
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        num = num * 10 + (*s - '0');
        s++;
    }

    return num * sign;
}

void itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
  
  /*  If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;
      
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;
  
  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}
