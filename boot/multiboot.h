// multiboot.h
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

// Флаги в заголовке multiboot
#define MULTIBOOT_INFO_MEMORY          0x00000001
#define MULTIBOOT_INFO_BOOTDEV         0x00000002
#define MULTIBOOT_INFO_CMDLINE         0x00000004
#define MULTIBOOT_INFO_MODS            0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS       0x00000010
#define MULTIBOOT_INFO_ELF_SHDR        0x00000020
#define MULTIBOOT_INFO_MEM_MAP         0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO      0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE    0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME 0x00000200
#define MULTIBOOT_INFO_APM_TABLE       0x00000400
#define MULTIBOOT_INFO_VBE_INFO        0x00000800

typedef struct multiboot_info {
    unsigned int flags;
    unsigned int mem_lower;
    unsigned int mem_upper;
    unsigned int boot_device;
    unsigned int cmdline;
    unsigned int mods_count;
    unsigned int mods_addr;
    unsigned int syms[4];
    unsigned int mmap_length;
    unsigned int mmap_addr;
    unsigned int drives_length;
    unsigned int drives_addr;
    unsigned int config_table;
    unsigned int boot_loader_name;
    unsigned int apm_table;
    unsigned int vbe_control_info;
    unsigned int vbe_mode_info;
    unsigned int vbe_mode;
    unsigned int vbe_interface_seg;
    unsigned int vbe_interface_off;
    unsigned int vbe_interface_len;
} __attribute__((packed)) multiboot_info_t;

#endif