# FurryOS

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

FurryOS - это Unix-подобная операционная система, разрабатываемая мной для получения себе опыта в системном программировании и углубления знаний языка C.

## Сборка и запуск

### Клонирование и сборка
```bash
git clone https://github.com/kroshhaker/FurryOS.git
cd FurryOS
make
```

### Работа с QEMU
Доступны вспомогательные скрипты для работы с виртуальной машиной:
```bash
./scripts/mount.sh    # Монтирование mydisk.img в /mnt
make install          # Копирование ядра в /mnt/boot/
make run              # Запуск QEMU с вашей ОС
```

## Настройка диска

### Создание образа диска
```bash
qemu-img create -f raw mydisk.img 1G
```

### Разметка диска
Рекомендуемая схема разделов (MBR):
1. 128MiB /boot (ext2)
2. Остальное пространство / (ext2)

Пример создания разделов:
```bash
losetup -fP mydisk.img 
lsblk
fdisk /dev/loopX  # Замените X на ваш номер устройства
```

## Файловая система
Проект следует стандарту FHS (Filesystem Hierarchy Standard). Готовый образ файловой системы доступен в файле `furryos.tar.xz`.