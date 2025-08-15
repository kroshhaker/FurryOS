#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

struct file;
struct inode;
struct dirent;

struct filesystem {
    const char *name;
    uint32_t signature_offset;
    uint8_t signature[4];
    uint8_t sig_len;
};

#define FS_COUNT (sizeof(filesystems)/sizeof(filesystems[0]))


struct file_ops {
    int (*read)(struct file*, void*, size_t);
    int (*write)(struct file*, const void*, size_t);
    int (*open)(struct inode*);
    int (*close)(struct inode*);
    int (*readdir)(struct file*, struct dirent*);
};

extern void detect_filesystem(uint32_t start_lba);

#endif