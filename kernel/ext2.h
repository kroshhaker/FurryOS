#ifndef _EXT2_H
#define _EXT2_H

#include <stdint.h>

typedef struct {
    uint32_t s_inodes_count;        /* Общее количество инодов */
    uint32_t s_blocks_count;        /* Общее количество блоков */
    uint32_t s_r_blocks_count;      /* Резервные блоки (не используются) */
    uint32_t s_free_blocks_count;   /* Количество свободных блоков */
    uint32_t s_free_inodes_count;   /* Количество свободных инодов */
    uint32_t s_first_data_block;    /* Номер первого данных блока */
    uint32_t s_log_block_size;      /* Логарифм размера блока (в 1024 байтах) */
    uint32_t s_log_frag_size;       /* Логарифм размера фрагмента (не используется) */
    uint32_t s_blocks_per_group;    /* Количество блоков в группе */
    uint32_t s_frags_per_group;     /* Количество фрагментов в группе (не используется) */
    uint32_t s_inodes_per_group;    /* Количество инодов в группе */
    uint32_t s_mtime;               /* Время последней проверки файловой системы */
    uint32_t s_wtime;               /* Время последнего монтирования */
    uint16_t s_mnt_count;           /* Счётчик монтирований */
    uint16_t s_max_mnt_count;       /* Максимальное количество монтирований до проверки */
    uint16_t s_magic;               /* Магическое число (0xEF53) */
    uint16_t s_state;               /* Состояние файловой системы */
    uint16_t s_errors;              /* Поведение при ошибках */
    uint16_t s_minor_rev_level;     /* Минимальная версия */
    uint32_t s_lastcheck;           /* Время последней проверки */
    uint32_t s_checkinterval;       /* Интервал между проверками */
    uint32_t s_creator_os;          /* Операционная система, создавшая файловую систему */
    uint32_t s_rev_level;           /* Уровень ревизии */
    uint16_t s_def_resuid;          /* Идентификатор пользователя по умолчанию */
    uint16_t s_def_resgid;          /* Идентификатор группы по умолчанию */
    uint32_t s_first_ino;           /* Номер первого инода */
    uint16_t s_inode_size;          /* Размер инода */
    uint16_t s_block_group_nr;      /* Номер группы блоков */
    uint32_t s_feature_compat;      /* Совместимые особенности */
    uint32_t s_feature_incompat;    /* Несовместимые особенности */
    uint32_t s_feature_ro_compat;   /* Только для чтения особенности */
    uint8_t  s_uuid[16];            /* UUID файловой системы */
    char     s_volume_name[16];     /* Имя тома */
    char     s_last_mounted[64];    /* Последний путь монтирования */
    uint32_t s_algorithm_usage_bitmap; /* Битмап используемых алгоритмов */
    uint8_t  s_prealloc_blocks;     /* Количество предварительно выделенных блоков */
    uint8_t  s_prealloc_dir_blocks; /* Количество предварительно выделенных блоков для директорий */
    uint16_t s_padding1;            /* Заполнитель */
    uint32_t s_reserved[204];       /* Резерв */
} __attribute__((packed)) ext2_super_block_t;

void parse_superblock(uint32_t start_sb_lba, ext2_super_block_t *sb);

#endif