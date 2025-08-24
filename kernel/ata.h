#ifndef _ATA_H
#define _ATA_H

#include "stdint.h"

#define ATA_PRIMARY_IO       0x1F0
#define ATA_PRIMARY_CTRL     0x3F6
#define ATA_REG_DATA         (ATA_PRIMARY_IO + 0)
#define ATA_REG_ERROR        (ATA_PRIMARY_IO + 1)
#define ATA_REG_SECCOUNT0    (ATA_PRIMARY_IO + 2)
#define ATA_REG_LBA0         (ATA_PRIMARY_IO + 3)
#define ATA_REG_LBA1         (ATA_PRIMARY_IO + 4)
#define ATA_REG_LBA2         (ATA_PRIMARY_IO + 5)
#define ATA_REG_HDDEVSEL     (ATA_PRIMARY_IO + 6)
#define ATA_REG_COMMAND      (ATA_PRIMARY_IO + 7)
#define ATA_REG_STATUS       (ATA_PRIMARY_IO + 7)

#define ATA_CMD_READ_SECTORS 0x20
#define ATA_CMD_WRITE_SECTORS 0x30
#define ATA_SR_BSY  0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DRQ  0x08

void ata_read_sector(uint32_t lba, uint8_t *buffer);
void ata_read_sectors(uint32_t lba, uint8_t sector_count, uint8_t *buffer);
void ata_write_sector(uint32_t lba, uint8_t *buffer);
void ata_write_sectors(uint32_t lba, uint8_t sector_count, uint8_t *buffer);

#endif