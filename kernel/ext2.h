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

typedef struct {
    uint32_t block_bitmap;       /* Номер блока с битовой картой блоков */
    uint32_t inode_bitmap;       /* Номер блока с битовой картой инодов */
    uint32_t inode_table;        /* Начальный блок таблицы инодов */
    uint16_t free_blocks_count;  /* Кол-во свободных блоков в группе */
    uint16_t free_inodes_count;  /* Кол-во свободных инодов в группе */
    uint16_t used_dirs_count;    /* Кол-во каталогов в группе */
    uint8_t  reserved[14];
} __attribute__((packed)) ext2_block_group_descriptor_t;

typedef struct {
    uint16_t mode;             /* тип файла и права доступа */
    uint16_t uid;              /* ID пользователя (владельца) */
    uint32_t size_low;         /* младшие 32 бита размера файла */
    uint32_t atime;            /* время последнего доступа */
    uint32_t ctime;            /* время создания */
    uint32_t mtime;            /* время последнего изменения */
    uint32_t dtime;            /* время удаления */
    uint16_t gid;              /* ID группы */
    uint16_t links_count;      /* число hard links */
    uint32_t blocks;           /* число занятых секторов */
    uint32_t flags;            /* флаги */
    uint32_t osd1;             /* ОС-специфичное поле #1 */
    uint32_t block[15];        /* указатели на блоки */
    uint32_t generation;       /* generation (для NFS) */
    uint32_t file_acl;         /* ACL или расширенные атрибуты */
    uint32_t size_high;        /* старшие 32 бита размера / Directory ACL */
    uint32_t fragment_addr;    /* адрес фрагмента */
    uint8_t  osd2[12];         /* ОС-специфичное поле #2 */
} __attribute__((packed)) ext2_inode_t;

void read_superblock(uint32_t start_sb_lba, ext2_super_block_t *sb);
void read_bg_desc (uint32_t start_block, ext2_block_group_descriptor_t *bgd);
void read_inode (uint32_t start_block, ext2_super_block_t *sb, ext2_block_group_descriptor_t *bgdt, ext2_inode_t *inode, uint8_t n);
void read_bitmap(uint32_t start_block, ext2_super_block_t *sb, ext2_block_group_descriptor_t *bgdt, uint8_t type, uint8_t *buffer);
void set_bit(uint32_t start_block, ext2_super_block_t *sb, ext2_block_group_descriptor_t *bgdt, uint8_t type, uint8_t *bitmap, uint32_t index, uint8_t value);
int test_bit(uint8_t *bitmap, uint32_t index);

#endif