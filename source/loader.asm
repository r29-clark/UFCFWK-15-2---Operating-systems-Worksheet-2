
global loader

extern kernel_main

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .text
align 4

dd MAGIC_NUMBER
dd FLAGS
dd CHECKSUM

loader:
    ; Set up the stack
    mov esp, stack_top

    ; Call the main C kernel function
    call kernel_main

.loop:
    jmp .loop

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .note.GNU-stack noalloc noexec nowrite progbits
