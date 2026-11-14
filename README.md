# UFCFWK-15-2---Operating-systems-Worksheet-2

Task 1 - Setting up a small operating system

Objective: The objective of this task was to create the basic structure of an operating system and get it to boot using the GRUB in QEMU.

The linker script controls how the memory is arranged in the compiled kernel, the kernel is loaded at a certain address so that GRUB knows the expected memory location. The main part of the script is:

```ld
ENTRY(loader)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}
```

The Makefile automates building the operating system, it contains instructions to assemble, linking and then creating a bootable ISO image.

The main commands used for the makefile are:

```makefile
source/loader.o: source/loader.asm
	$(AS) -f elf source/loader.asm -o source/loader.o

kernel.elf: source/loader.o source/link.ld
	$(LD) -T source/link.ld -melf_i386 source/loader.o -o kernel.elf
```

The Assembly file is assembled into an object file. The linker then combines the object files using the linker script to produce `kernel.elf`.

Finally, `genisoimage` packages the required boot files into `os.iso`, which can be used to boot the operating system in QEMU.

GRUB is the bootloader to load the kernel into memory and start the operating system. GRUB uses the menu.lst file to find the kernel. The file 'stage2_eltorito' provides the GRUB bootloader needed to boot the ISO image.

1. QEMU starts the virutal machine using the ISO file
2. GRUB starts using the bootlaoder files included in the ISO.
3. GRUB reads the menu.lst file to find the kernel
4. GRUB loads the 'kernel.elf' into memory
5. The Assembly loader begins executing.
6. The loader places `0xCAFEBABE` into EAX and enters an infinite loop.

Testing Task 1

The operating system was built using the makefile and successfully booted in QEMU.

The IOS loaded into the machine allowing GRUB to load the kernel.

![alt text](screenshots/task1.png)