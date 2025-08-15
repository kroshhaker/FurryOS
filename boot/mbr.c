#include "mbr.h"

#include "console.h"
#include "ata.h"

void parse_mbr_partition(const uint8_t num, const uint8_t *buffer, mbr_t *mbr) {
    mbr_partition_t *part = &mbr->partitions[num];
    if (num >= 4) {
        part->boot_flag = 0;
        part->type = 0;
        part->start_lba = 0;
        part->size = 0;
        for (int i = 0; i < 3; i++) {
            part->start_chs[i] = 0;
            part->end_chs[i] = 0;
        }
        return;
    }
    uint16_t offset = 446 + (num * 16);

    if (offset + 15 >= 512) {
        part->boot_flag = 0;
        part->type = 0;
        part->start_lba = 0;
        part->size = 0;
        for (int i = 0; i < 3; i++) {
            part->start_chs[i] = 0;
            part->end_chs[i] = 0;
        }
        return;
    }

    part->boot_flag = buffer[offset + 0];
    
    part->start_chs[0] = buffer[offset + 1];
    part->start_chs[1] = buffer[offset + 2];
    part->start_chs[2] = buffer[offset + 3];
    
    part->type = buffer[offset + 4];

    part->end_chs[0] = buffer[offset + 5];
    part->end_chs[1] = buffer[offset + 6];
    part->end_chs[2] = buffer[offset + 7];

    part->start_lba = ((uint32_t)buffer[offset + 8])  |
                     ((uint32_t)buffer[offset + 9]  << 8)  |
                     ((uint32_t)buffer[offset + 10] << 16) |
                     ((uint32_t)buffer[offset + 11] << 24);

    part->size = ((uint32_t)buffer[offset + 12]) |
                ((uint32_t)buffer[offset + 13] << 8)  |
                ((uint32_t)buffer[offset + 14] << 16) |
                ((uint32_t)buffer[offset + 15] << 24);

    return;
}

void parse_mbr(mbr_t *mbr) {
    static uint8_t mbr_sector[512];
    ata_read_sector(0, mbr_sector);
    
    if (!(mbr_sector[510] == 0x55 && mbr_sector[511] == 0xAA)) {
        print_colored("ERROR. Invalid mbr signature!", RED);
        return;
    }

    for (int i = 0; i < 400; i++) {
        mbr->bootstrap[i] = mbr_sector[i];
    }

    mbr->udid = ((uint32_t)mbr_sector[440]) |
            ((uint32_t)mbr_sector[441] << 8) |
            ((uint32_t)mbr_sector[442] << 16) |
            ((uint32_t)mbr_sector[443] << 24);

    mbr->mode = ((uint8_t)mbr_sector[445] << 8) |
                ((uint8_t)mbr_sector[444]);

    for (int i = 0; i < 4; i++) {
        parse_mbr_partition(i, mbr_sector, mbr);
    }

    mbr->signature = (uint16_t)mbr_sector[510] | ((uint16_t)mbr_sector[511] << 8);
}