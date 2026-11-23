#include "interrupts.h"
#include "pic.h"
#include "keyboard.h"
#include "hardware_interrupt_enabler.h"
#include "framebuffer.h"
#include "../source/io.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
#define INPUT_BUFFER_SIZE 256

static struct IDTDescriptor idt_descriptors[256];
static struct IDT idt;

static volatile u8int input_buffer[INPUT_BUFFER_SIZE];
static volatile u32int buffer_head = 0;
static volatile u32int buffer_tail = 0;

static void buffer_put(u8int c)
{
    u32int next = (buffer_head + 1) % INPUT_BUFFER_SIZE;

    if (next != buffer_tail) {
        input_buffer[buffer_head] = c;
        buffer_head = next;
    }
}

int getc(void)
{
    int result;

    disable_hardware_interrupts();

    if (buffer_head == buffer_tail) {
        enable_hardware_interrupts();
        return -1;
    }

    result = input_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % INPUT_BUFFER_SIZE;

    enable_hardware_interrupts();
    return result;
}

void interrupts_install_idt(void)
{
    u32int address = (u32int)interrupt_handler_33;

    for (int i = 0; i < 256; i++) {
        idt_descriptors[i].offset_low = 0;
        idt_descriptors[i].offset_high = 0;
        idt_descriptors[i].segment_selector = 0;
        idt_descriptors[i].reserved = 0;
        idt_descriptors[i].type_and_attr = 0;
    }

    idt_descriptors[33].offset_low = address & 0xFFFF;
    idt_descriptors[33].offset_high = (address >> 16) & 0xFFFF;
    idt_descriptors[33].segment_selector = 0x08;
    idt_descriptors[33].reserved = 0;
    idt_descriptors[33].type_and_attr = 0x8E;

    idt.size = sizeof(idt_descriptors) - 1;
    idt.address = (u32int)idt_descriptors;

    load_idt((u32int)&idt);

    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

void interrupt_handler(
    __attribute__((unused)) struct cpu_state cpu,
    u32int interrupt,
    __attribute__((unused)) struct stack_state stack)
{
    if (interrupt == INTERRUPTS_KEYBOARD) {
        while (inb(0x64) & 1) {
            u8int scan = keyboard_read_scan_code();

            if (!(scan & 0x80)) {
                u8int ascii = keyboard_scan_code_to_ascii(scan);

                if (ascii == '\b') {
                    fb_backspace();
                    buffer_put(ascii);
                } else if (ascii == '\n') {
                    fb_newline();
                    buffer_put(ascii);
                } else if (ascii != 0) {
                    fb_write_char(ascii);
                    buffer_put(ascii);
                }
            }
        }

        pic_acknowledge(interrupt);
    }
}