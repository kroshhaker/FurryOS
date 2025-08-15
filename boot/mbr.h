#ifndef _MBR_H
#define _MBR_H

#include "stdint.h"

typedef struct {
    uint8_t boot_flag;       // 0x80 = bootable
    uint8_t start_chs[3];    // CHS начала
    uint8_t type;            // тип раздела
    uint8_t end_chs[3];      // CHS конца
    uint32_t start_lba;      // LBA начала
    uint32_t size;           // размер в секторах
} __attribute__((packed)) mbr_partition_t;

typedef struct {
    uint8_t bootstrap[440];         // код загрузчика
    uint32_t udid;
    uint16_t mode;
    mbr_partition_t partitions[4];  // 4 записи разделов
    uint16_t signature;             // 0xAA55
} __attribute__((packed)) mbr_t;

void parse_mbr_partition(const uint8_t num, const uint8_t *buffer, mbr_t *mbr);
void parse_mbr(mbr_t *mbr);

#endif