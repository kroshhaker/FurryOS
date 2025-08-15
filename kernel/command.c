#include "command.h"

#include "ctype.h"
#include "stdint.h"
#include "stddef.h"
#include "console.h"
#include "mbr.h"
#include "string.h"
#include "io.h"
#include "ata.h"
#include "convert.h"
#include "filesystem.h"

// ==== Обработчики ====
void cmd_help(char *args) {
    print("Available commands:\n");
    for (int i = 0; commands[i].name; i++) {
        print("  ");
        print(commands[i].name);
        print(" - ");
        print(commands[i].help);
        print("\n");
    }
}

void cmd_clear(char *args) {
    clear_screen();
}

void cmd_echo(char *args) {
    print(args);
    println("");
}

void cmd_color(char *args) {
    print("VGA color test:\n");
    for (int i = 0; i < 16; i++) {
        print_colored("Color ", i | (BLACK << 4));
        print_num(i);
        print(" ");
    }
    println("");
}

void cmd_reboot(char *args) {
    print_colored("Rebooting system...\n", LIGHT_RED | (BLACK << 4));
    outb(0x64, 0xFE);
}

void cmd_halt(char *args) {
    print_colored("System halted. It is now safe to power off.\n", LIGHT_RED | (BLACK << 4));
    asm volatile("cli; hlt");
}


// START DISK

void cmd_disk_help(char *args) {
    print("disk commands:\n");
    for (int i = 0; disk_cmds[i].name; i++) {
        print("  ");
        print(disk_cmds[i].name);
        print(" - ");
        print(disk_cmds[i].help);
        print("\n");
    }
}

void cmd_disk_sector(char *args) {
    if (!args) return;

    size_t len = strlen(args);
    unsigned char str[strlen(args)+1];
    for (int i = 0; i < len; i++) {
        if (!(args[i] >= '0' && args[i] <= '9')) {
            print("Usage: disk sector <DEC>\n");
            return;
        }
        str[i] = args[i];
    }
    str[len] = '\0';
    uint32_t lba = (uint32_t)s2i(str);
    uint8_t buffer[512];
    ata_read_sector(lba, buffer);
    print_hex_buffer(buffer, 512);
    println("");
}

void cmd_disk_byte(char *args) {
    if (!args) return;

    size_t len = strlen(args);
    unsigned char address[strlen(args)+1];
    for (int i = 0; i < len; i++) {
        if (!isxdigit((unsigned char)args[i])) {
            print("Usage: disk byte <HEX>\n");
            return;
        }
        address[i] = args[i];
    }
    address[len] = '\0';
    uint32_t addr_val = hex_to_dec(address);
    uint32_t lba = addr_val / 512;
    uint32_t byte = addr_val % 512;
    uint8_t buffer[512];
    ata_read_sector(lba, buffer);

    print("0x");
    print(address);
    print(" = 0x");

    char out[3];
    out[0] = "0123456789ABCDEF"[buffer[byte] >> 4];
    out[1] = "0123456789ABCDEF"[buffer[byte] & 0x0F];
    out[2] = '\0';
    print(out);
    print("\n");
}

void cmd_disk_part_list(char *args) {
    static mbr_t disk;
    parse_mbr(&disk);
    print("boot | start chs |  end chs | type |  start lba |       size |\n");
    for (int i = 0; i < 4; i++) {
        print_num_width(disk.partitions[i].boot_flag, 4);
        print(" | ");
        print_hex_buffer(disk.partitions[i].start_chs, 3);
        print(" | ");
        print_hex_buffer(disk.partitions[i].end_chs, 3);
        print("|  ");
        print_hex8(disk.partitions[i].type);
        print("  | ");
        print_num_width(disk.partitions[i].start_lba, 10);
        print(" | ");
        print_num_width(disk.partitions[i].size, 10);
        print(" |\n");            
    }
}

void cmd_disk_part_help(char *args) {
    print("disk part commands:\n");
    for (int i = 0; disk_part_cmds[i].name; i++) {
        print("  ");
        print(disk_part_cmds[i].name);
        print(" - ");
        print(disk_part_cmds[i].help);
        print("\n");
    }
}

command_t disk_part_cmds[] = {
    { "help", cmd_disk_part_help, "show disk part commands" },
    { "list", cmd_disk_part_list, "list partitions" },
    { NULL, NULL, NULL }
};

void cmd_disk_part(char *args) {
    if (!args || args[0] == '\0') {
        print("Usage: disk part <command>\n");
        return;
    }

    char *sub = strtok(args, " ");
    char *sub_args = strtok(NULL, "");

    int found = 0;
    for (int i = 0; disk_part_cmds[i].name; i++) {
        if (strcmp(sub, disk_part_cmds[i].name) == 0) {
            disk_part_cmds[i].func(sub_args ? sub_args : "");
            found = 1;
            break;
        }
    }

    if (!found) {
        print_colored("Unknown disk command: ", LIGHT_RED | (BLACK << 4));
        print(sub);
        print("\nType 'disk part help' for a list of commands\n");
    }
}

command_t disk_cmds[] = {
    { "help", cmd_disk_help, "show disk commands" },
    { "part", cmd_disk_part, "partition operations" },
    { "byte", cmd_disk_byte, "read byte by HEX address" },
    { "sector", cmd_disk_sector, "read sector"},
    { NULL, NULL, NULL }
};

void cmd_disk(char *args) {
    if (!args || args[0] == '\0') {
        print("Usage: disk <command>\n");
        return;
    }

    char *sub = strtok(args, " ");
    char *sub_args = strtok(NULL, "");

    int found = 0;
    for (int i = 0; disk_cmds[i].name; i++) {
        if (strcmp(sub, disk_cmds[i].name) == 0) {
            disk_cmds[i].func(sub_args ? sub_args : "");
            found = 1;
            break;
        }
    }

    if (!found) {
        print_colored("Unknown disk command: ", LIGHT_RED | (BLACK << 4));
        print(sub);
        print("\nType 'disk help' for a list of commands\n");
    }
}

// END DISK


// START FS

void cmd_fs_help(char *args) {
    print("fs commands:\n");
    for (int i = 0; fs_cmds[i].name; i++) {
        print("  ");
        print(fs_cmds[i].name);
        print(" - ");
        print(fs_cmds[i].help);
        print("\n");
    }
}

void cmd_fs_check(char *args) {
    if (!args) return;

    size_t len = strlen(args);

    if (len == 0 || args[0] < '1' || args[0] > '4') {
        println("Usage: fs check <1-4>");
        return;
    }

    uint8_t part = (args[0] - '0') - 1;

    static mbr_t disk;
    parse_mbr(&disk);
    detect_filesystem(disk.partitions[part].start_lba);
}

command_t fs_cmds[] = {
    { "help",   cmd_fs_help,   "show this help" },
    { "check",   cmd_fs_check,   "check filesystem" },
};

void cmd_fs(char *args) {
    if (!args || args[0] == '\0') {
        print("Usage: fs <command>\n");
        return;
    }

    char *sub = strtok(args, " ");
    char *sub_args = strtok(NULL, "");

    int found = 0;
    for (int i = 0; fs_cmds[i].name; i++) {
        if (strcmp(sub, fs_cmds[i].name) == 0) {
            fs_cmds[i].func(sub_args ? sub_args : "");
            found = 1;
            break;
        }
    }

    if (!found) {
        print_colored("Unknown fs command: ", LIGHT_RED | (BLACK << 4));
        print(sub);
        print("\nType 'fs help' for a list of commands\n");
    }
}

// END FS

// ==== Таблица команд ====
command_t commands[] = {
    { "help",   cmd_help,   "show this help" },
    { "clear",  cmd_clear,  "clear the screen" },
    //{ "sysinfo",cmd_sysinfo,"show system information" },
    { "disk", cmd_disk, "disk operations" },
    { "fs", cmd_fs, "filesystem operations" },
    { "echo",   cmd_echo,   "print text" },
    { "color",  cmd_color,  "color test" },
    { "reboot", cmd_reboot, "reboot" },
    { "halt",   cmd_halt,   "halt the system" },
    { NULL, NULL, NULL }
};