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