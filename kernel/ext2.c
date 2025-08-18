#include "ext2.h"

#include "ata.h"
#include "string.h"

void parse_superblock(uint32_t start_sb_lba, ext2_super_block_t *sb) {
    uint8_t buffer[1024];
    ata_read_sectors(start_sb_lba, 2, buffer);
    memcpy(sb, buffer, sizeof(ext2_super_block_t));
}

uint32_t block_size(uint32_t start_lba) {
    uint8_t buffer[512];
    ata_read_sector(start_lba + 2, buffer);
    uint32_t s_log_block_size = ((uint32_t)buffer[24]) |
                                ((uint32_t)buffer[25] << 8) |
                                ((uint32_t)buffer[26] << 16) |
                                ((uint32_t)buffer[27] << 24);
    uint32_t size = 1024 << s_log_block_size;
    return size;
}
