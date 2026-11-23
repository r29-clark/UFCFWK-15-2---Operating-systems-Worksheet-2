
global inb
global outb

; Read a byte from an I/O port
inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

; Write a byte to an I/O port
outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret
