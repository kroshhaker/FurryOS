#include "ata.h"

#include "io.h"
#include "console.h"
#include "kernel.h"

static int ata_wait(uint8_t mask, uint8_t value) {
    int timeout = 1000000;
    while (((inb(ATA_REG_STATUS) & mask) != value) && --timeout);
    return timeout > 0;
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    int timeout = 1000000;
    while ((inb(ATA_REG_STATUS) & ATA_SR_BSY) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_read_sector timeout\n", RED);
        print("ATA device did not clear BSY flag in STATUS register. Sector read aborted.\n");
        return;
    }

    // Отправляем LBA
    outb(ATA_REG_SECCOUNT0, 1);                             // кол-во секторов = 1
    outb(ATA_REG_LBA0, (uint8_t) lba);                      // LBA low byte
    outb(ATA_REG_LBA1, (uint8_t)(lba >> 8));                // LBA mid byte
    outb(ATA_REG_LBA2, (uint8_t)(lba >> 16));               // LBA high byte
    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));    // device

    // Команда READ SECTORS
    outb(ATA_REG_COMMAND, ATA_CMD_READ_SECTORS);

    timeout = 1000000;

    while (!(inb(ATA_REG_STATUS) & ATA_SR_DRQ) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_read_sector timeout\n", RED);
        print("Failed to read sector from ATA device.\n");
        return;
    }

    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_REG_DATA);
        buffer[i*2]   = data & 0xFF;
        buffer[i*2+1] = data >> 8;
    }
}

void ata_read_sectors(uint32_t lba, uint8_t sector_count, uint8_t *buffer) {
    int timeout = 1000000;
    while ((inb(0x1F7) & ATA_SR_BSY) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_read_sector timeout\n", RED);
        panic("parse_mbr crash\n");
        return;
    }

    outb(ATA_REG_SECCOUNT0, sector_count);

    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));

    outb(ATA_REG_COMMAND, ATA_CMD_READ_SECTORS);

    for (uint8_t s = 0; s < sector_count; s++) {
        timeout = 1000000;
        while (!(inb(0x1F7) & ATA_SR_DRQ) && timeout--);
        
        if (timeout <= 0) {
            print_colored("ERROR: ata_read_sector timeout\n", RED);
            print("Failed to read sector from ATA device.\n");
            return;
        }

        for (int i = 0; i < 256; i++) { // 256 слов = 512 байт
            uint16_t data = inw(ATA_REG_DATA);
            buffer[s*512 + i*2]   = data & 0xFF;
            buffer[s*512 + i*2+1] = data >> 8;
        }
    }
}

void ata_write_sector(uint32_t lba, uint8_t *buffer) {
    int timeout = 1000000;
    while ((inb(ATA_REG_STATUS) & ATA_SR_BSY) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_write_sector timeout\n", RED);
        print("ATA device did not clear BSY flag in STATUS register. Sector read aborted.\n");
        return;
    }

    outb(ATA_REG_SECCOUNT0, 1);
    outb(ATA_REG_LBA0, (uint8_t) lba);
    outb(ATA_REG_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_REG_LBA2, (uint8_t)(lba >> 16));
    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));

    outb(ATA_REG_COMMAND, ATA_CMD_WRITE_SECTORS);

    timeout = 1000000;

    while (!(inb(ATA_REG_STATUS) & ATA_SR_DRQ) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_write_sector timeout\n", RED);
        print("Failed to write sector from ATA device.\n");
        return;
    }

    for (int i = 0; i < 256; i++) {
        outw(ATA_REG_DATA, ((uint16_t*)buffer)[i]);
    }

    timeout = 1000000;
    while ((inb(ATA_REG_STATUS) & ATA_SR_BSY) && timeout--);
    if (timeout <= 0) {
        print_colored("ERROR: ata_write_sector timeout\n", RED);
        print("ATA device did not complete sector write (BSY stuck).\n");
    }
}

void ata_write_sectors(uint32_t lba, uint8_t sector_count, uint8_t *buffer) {
    int timeout = 1000000;
    while ((inb(0x1F7) & ATA_SR_BSY) && timeout--);

    if (timeout <= 0) {
        print_colored("ERROR: ata_write_sector timeout\n", RED);
        print("ATA device did not clear BSY flag in STATUS register. Sector read aborted.\n");
        return;
    }

    outb(ATA_REG_SECCOUNT0, sector_count);

    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));

    outb(ATA_REG_COMMAND, ATA_CMD_WRITE_SECTORS);

    for (uint8_t s = 0; s < sector_count; s++) {
        timeout = 1000000;
        while (!(inb(0x1F7) & ATA_SR_DRQ) && timeout--);
        
        if (timeout <= 0) {
            print_colored("ERROR: ata_write_sector timeout\n", RED);
            print("Failed to write sector from ATA device.\n");
            return;
        }

        for (int i = 0; i < 256; i++) {
            outw(ATA_REG_DATA, ((uint16_t*)buffer)[i + s*512]);
        }

        timeout = 1000000;
        while ((inb(ATA_REG_STATUS) & ATA_SR_BSY) && timeout--);
        if (timeout <= 0) {
            print_colored("ERROR: ata_write_sector timeout\n", RED);
            print("ATA device did not complete sector write (BSY stuck).\n");
        }
    }
}