AS = nasm
CC = gcc
LD = ld
GENISOIMAGE = genisoimage
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -Wall -Wextra

OBJECTS = source/loader.o \
          source/kernel.o \
          source/io.o \
          drivers/framebuffer.o \
          drivers/pic.o \
          drivers/interrupts.o \
          drivers/keyboard.o \
          drivers/input.o \
          drivers/terminal.o \
          drivers/interrupt_handlers.o \
          drivers/interrupt_asm.o \
          drivers/hardware_interrupt_enabler.o

all: os.iso

%.o: %.asm
	$(AS) -f elf $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJECTS) source/link.ld
	$(LD) -T source/link.ld -melf_i386 $(OBJECTS) -o kernel.elf

iso/boot/grub/stage2_eltorito:
	cp /opt/os/stage2_eltorito iso/boot/grub/stage2_eltorito

iso/boot/kernel.elf: kernel.elf
	cp kernel.elf iso/boot/kernel.elf

os.iso: kernel.elf iso/boot/kernel.elf iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst
	$(GENISOIMAGE) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

run: os.iso
	$(QEMU) -display curses -monitor none -serial none -boot d -cdrom os.iso -m 32

clean:
	rm -f $(OBJECTS) kernel.elf iso/boot/kernel.elf os.iso logQ.txt

.PHONY: all run clean