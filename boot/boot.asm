[BITS 32]

section .multiboot
align 8
    dd 0xe85250d6
    dd 0
    dd header_end - header_start  
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

header_start:
    dw 0
    dw 0
    dd 8

header_end:

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top

    push ebx
    push eax
    call kernel_main

.halt:
    cli
    hlt
    jmp .halt