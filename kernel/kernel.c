#include "kernel.h"

#include "multiboot.h"
#include "console.h"
#include "command.h"
#include "stddef.h"
#include "string.h"

volatile unsigned int timer_ticks = 0;

void pit_handler() {
    timer_ticks++;
}

void sleep_ms(unsigned int ms) {
    unsigned int start = timer_ticks;
    while (timer_ticks - start < ms);
}

void panic(const char *msg) {
    print_colored("KERNEL PANIC: ", LIGHT_RED | (BLACK << 4));
    print(msg);
    print("\nSYSTEM HALTED.\n");
    print("Reboot the system to continue.\n");
    asm volatile("cli; hlt");
}

// Shell
void shell() {
    char input[256];

    while (1) {
        print_colored("FurrOS> ", LIGHT_BLUE | (BLACK << 4));
        getline(input, sizeof(input));
        println("");

        if (input[0] == '\0')
            continue;

        char *cmd = strtok(input, " ");
        char *args = strtok(NULL, "");

        int found = 0;
        for (int i = 0; commands[i].name; i++) {
            if (strcmp(cmd, commands[i].name) == 0) {
                commands[i].func(args ? args : "");
                found = 1;
                break;
            }
        }

        if (!found) {
            print_colored("Unknown command: ", LIGHT_RED | (BLACK << 4));
            print(cmd);
            print("\nType 'help' for a list of commands\n");
        }
    }
}


void kernel_main(unsigned int magic) {
    clear_screen();
    
    print_colored("=====================================\n", LIGHT_CYAN | (BLACK << 4));
    print_colored("         Welcome to Furry OS       \n", WHITE | (BLACK << 4));
    print_colored("=====================================\n", LIGHT_CYAN | (BLACK << 4));
    print("Version: 2.0 (GRUB)\n");
    
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    print_colored("ERROR: Invalid multiboot magic number!\n", LIGHT_RED | (BLACK << 4));
    return;
}

print_colored("Multiboot loader detected\n", LIGHT_GREEN | (BLACK << 4));
print_colored("Protected mode is active\n", LIGHT_GREEN | (BLACK << 4));
print_colored("VGA text mode initialized\n", LIGHT_GREEN | (BLACK << 4));
print_colored("Keyboard is ready\n", LIGHT_GREEN | (BLACK << 4));

print("\nType 'help' for a list of commands\n");
print("Type 'sysinfo' for system information\n\n");
    
    shell();
}