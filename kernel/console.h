#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Цвета VGA
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

#define DEFAULT_TEXT_COLOR LIGHT_GREY

struct vga_char {
    char character;
    char attribute;
} __attribute__((packed));

extern void clear_screen(void);
extern void getline(char* buffer, int max_len);
extern void putchar(char c);
extern void print(const char *str);
extern void print_colored(const char* str, unsigned char color);
extern void print_hex8(uint8_t num);
extern void print_hex_buffer(const unsigned char *buf, unsigned int len);
extern void print_num(int num);
extern void print_num_width(uint32_t num, int width);
extern void println(const char *str);
extern void scroll(void);
extern void set_cursor(int x, int y);

#endif