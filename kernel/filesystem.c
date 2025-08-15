#include "filesystem.h"

#include "ata.h"
#include "console.h"

struct filesystem filesystems[] = {
    {"ext2", 0x438, {0x53, 0xEF}, 2},
    {"FAT16", 0x36, {'F','A','T','1'}, 4},
    {"FAT32", 0x52, {'F','A','T','3'}, 4},
    {"NTFS", 0x03, {'N','T','F','S'}, 4}
};

bool match_signature(uint8_t *buffer, struct filesystem *fs) {
    for (uint8_t i = 0; i < fs->sig_len; i++) {
        if (buffer[fs->signature_offset + i] != fs->signature[i])
            return false;
    }

    return true;
}

void detect_filesystem(uint32_t start_lba) {
    uint8_t buffer[512 * 3];

    ata_read_sectors(start_lba, 3, buffer);

    for (int i = 0; i < FS_COUNT; i++) {
        if (match_signature(buffer, &filesystems[i])) {
            print("Filesystem: ");
            println(filesystems[i].name);
            return;
        }
    }

    println("Unknown filesystem");
}