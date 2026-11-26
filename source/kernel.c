#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"
#include "../drivers/terminal.h"

void kernel_main(void)
{
    fb_clear();
    fb_set_colour(15, 0);

    interrupts_install_idt();
    enable_hardware_interrupts();

    terminal_run();

    while (1) {
        __asm__ volatile ("hlt");
    }
}