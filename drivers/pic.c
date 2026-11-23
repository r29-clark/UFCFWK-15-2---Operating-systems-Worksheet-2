#include "pic.h"
#include "../source/io.h"

void pic_remap(s32int offset1, s32int offset2)
{
    u8int mask1 = inb(PIC_1_DATA);
    u8int mask2 = inb(PIC_2_DATA);

    outb(PIC_1_COMMAND, 0x11);
    outb(PIC_2_COMMAND, 0x11);

    outb(PIC_1_DATA, (u8int)offset1);
    outb(PIC_2_DATA, (u8int)offset2);

    outb(PIC_1_DATA, 0x04);
    outb(PIC_2_DATA, 0x02);

    outb(PIC_1_DATA, 0x01);
    outb(PIC_2_DATA, 0x01);

    outb(PIC_1_DATA, mask1);
    outb(PIC_2_DATA, mask2);
}

void pic_acknowledge(u32int interrupt)
{
    if (interrupt >= PIC_2_OFFSET &&
        interrupt <= PIC_2_OFFSET + 7)
    {
        outb(PIC_2_COMMAND, PIC_ACKNOWLEDGE);
    }

    if (interrupt >= PIC_1_OFFSET &&
        interrupt <= PIC_2_OFFSET + 7)
    {
        outb(PIC_1_COMMAND, PIC_ACKNOWLEDGE);
    }
}