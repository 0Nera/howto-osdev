#include <kernel.h>

volatile uint16_t *video_memory = (uint16_t*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;

void move_cursor() {
    uint16_t cursorLocation = cursor_y * VIDEO_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation);
}

void scroll() {
    uint16_t blank = 0x20 | (0x0F << 8);

    if (cursor_y >= VIDEO_HEIGHT) {
        int i;
        for (i = 0; i < (VIDEO_HEIGHT - 1) * VIDEO_WIDTH; i++) {
            video_memory[i] = video_memory[i + VIDEO_WIDTH];
        }

        for (i = (VIDEO_HEIGHT - 1) * VIDEO_WIDTH; i < VIDEO_HEIGHT * VIDEO_WIDTH; i++) {
            video_memory[i] = blank;
        }
        cursor_y = VIDEO_HEIGHT - 1;
    }
}

void print_char(char c, char color) {
    uint16_t attribute = color << 8;
    volatile uint16_t *location;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            print_char(' ', color);
            cursor_x--;
        }
    } else {
        location = video_memory + (cursor_y * VIDEO_WIDTH + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= VIDEO_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

void print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], 0x0F);
    }
}

void clear_screen() {
    uint16_t blank = 0x20 | (0x0F << 8);

    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void set_cursor_position(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    move_cursor();
}