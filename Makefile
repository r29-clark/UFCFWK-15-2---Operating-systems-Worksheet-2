
AS = nasm
CC = gcc
LD = ld
GENISOIMAGE = genisoimage
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -Wall -Wextra

all: os.iso

source/loader.o: source/loader.asm
	$(AS) -f elf source/loader.asm -o source/loader.o

source/kernel.o: source/kernel.c
	$(CC) $(CFLAGS) -c source/kernel.c -o source/kernel.o

drivers/framebuffer.o: drivers/framebuffer.c drivers/framebuffer.h
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

kernel.elf: source/loader.o source/kernel.o drivers/framebuffer.o source/link.ld
	$(LD) -T source/link.ld -melf_i386 source/loader.o source/kernel.o drivers/framebuffer.o -o kernel.elf

iso/boot/grub/stage2_eltorito:
	cp /opt/os/stage2_eltorito iso/boot/grub/stage2_eltorito

iso/boot/kernel.elf: kernel.elf
	cp kernel.elf iso/boot/kernel.elf

os.iso: kernel.elf iso/boot/kernel.elf iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst
	$(GENISOIMAGE) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

run: os.iso
	$(QEMU) -display curses -monitor telnet::45454,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32

clean:
	rm -f source/loader.o source/kernel.o drivers/framebuffer.o kernel.elf iso/boot/kernel.elf os.iso logQ.txt

.PHONY: all run clean
