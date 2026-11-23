#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"

void kernel_main(void)
{
    fb_clear();
    fb_set_colour(15, 0);

    fb_write("MyOS - Keyboard Test");
    fb_newline();
    fb_write("Type something:");
    fb_newline();

    interrupts_install_idt();
    enable_hardware_interrupts();

    while (1) {
        __asm__ volatile ("hlt");
    }
}