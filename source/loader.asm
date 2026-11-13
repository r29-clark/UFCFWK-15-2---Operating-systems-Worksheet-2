global loader

extern sum_of_three
extern multiply_two
extern subtract_two

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

    ; Call sum_of_three(1, 2, 3)
    push dword 3
    push dword 2
    push dword 1
    call sum_of_three
    add esp, 12

    ; Call multiply_two(4, 5)
    push dword 5
    push dword 4
    call multiply_two
    add esp, 8

    ; Call subtract_two(10, 4)
    push dword 4
    push dword 10
    call subtract_two
    add esp, 8

.loop:
    jmp .loop

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .note.GNU-stack noalloc noexec nowrite progbits