#ifndef __KERNEL_H
#define __KERNEL_H 1

#include <stdint.h>
#include <stddef.h>

#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// hal
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint8_t inb(uint16_t port);
void io_wait(void);

// video
void print(const char *str);
void print_char(char c, char color);

// kernel lib
void memset(uint8_t *addr, uint8_t p, size_t n);
int strncmp(const char *str1, const char *str2, size_t n);
int strcmp(const char *str1, const char *str2);

#endif