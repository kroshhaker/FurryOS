#!/bin/bash
qemu-system-x86_64 \
    -enable-kvm \
    -cpu host \
    -smp 4 \
    -m 1G \
    -drive file=mydisk.img,format=raw \
    -vga virtio \
    -display sdl