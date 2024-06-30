#include <kernel.h>

static unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`',   0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',   0,
    '*',
    0,
    ' ',
    0,
    0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0,   0,   0,
    0,
    0,
    0,
};

void read_line(char *buffer, size_t size) {
    size_t index = 0;
    while (1) {
        if (inb(KEYBOARD_STATUS_PORT) & 0x01) {
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            if (scancode < 128) {
                char key = keyboard_map[scancode];
                if (key == '\n') {
                    buffer[index] = '\0';
                    print("\n");
                    break;
                } else if (key == '\b') {
                    if (index > 0) {
                        index--;
                        print("\b \b");
                    }
                } else if (index < size - 1) {
                    buffer[index++] = key;
                    print_char(key, 0x0F);
                }
            }
        }
    }
}

void execute_command(const char *command) {
    if (strncmp(command, "echo ", 5) == 0) {
        print(command + 5);
        print("\n");
    } else if (strcmp(command, "exit") == 0) {
        print("Exiting terminal...\n");
        outw(0xB004, 0x2000);
        outw(0x604, 0x2000);
        outw(0x4004, 0x3400);
        outw(0x600, 0x34);
    } else {
        print("Unknown command: ");
        print(command);
        print("\n");
    }
}

void terminal() {
    char buffer[256];
    while (1) {
        print("> ");
        read_line(buffer, sizeof(buffer));
        execute_command(buffer);
    }
}

void kernel_main() {
    print("Hello DOS World!\n");
    terminal();

    for (;;) {
        asm volatile("hlt");
    }
}