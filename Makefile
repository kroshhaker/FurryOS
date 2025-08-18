CC = gcc
ASM = nasm
LD = ld

CFLAGS = -ffreestanding -fno-builtin -nostdlib -nostdinc \
         -fno-stack-protector -m32 -Iinclude -Ikernel -Iboot
CFLAGS_DEBUG = -ffreestanding -fno-builtin -nostdlib -nostdinc \
               -fno-stack-protector -m32 -Iinclude -Ikernel -Iboot -O0 -g -DDEBUG
ASMFLAGS = -f elf32
LDFLAGS = -T link.ld -nostdlib -m elf_i386

# Исходники
C_SOURCES := $(wildcard kernel/*.c) $(wildcard boot/*.c) $(wildcard include/*.c)
ASM_SOURCES := $(wildcard boot/*.asm)

# Соответствующие объектные файлы в build/
C_OBJECTS := $(patsubst %.c, build/%.o, $(notdir $(C_SOURCES)))
ASM_OBJECTS := $(patsubst %.asm, build/%.o, $(notdir $(ASM_SOURCES)))

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

all: kernel.bin

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: clean kernel.bin

# Сборка ASM
build/%.o: boot/%.asm
	@mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@

# Сборка C (kernel, boot, include)
build/%.o: kernel/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: boot/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: include/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка
kernel.bin: $(OBJECTS) link.ld
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

run:
	bash scripts/start.sh

clean:
	rm -rf build *.bin

install:
	rm -f /mnt/boot/kernel-001
	cp ./kernel.bin /mnt/boot/kernel-001

.PHONY: all run clean install
