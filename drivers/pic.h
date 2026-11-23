#ifndef INCLUDE_PIC_H
#define INCLUDE_PIC_H

#include "type.h"

#define PIC_1_COMMAND 0x20
#define PIC_1_DATA 0x21
#define PIC_2_COMMAND 0xA0
#define PIC_2_DATA 0xA1

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28
#define PIC_ACKNOWLEDGE 0x20

void pic_remap(s32int offset1, s32int offset2);
void pic_acknowledge(u32int interrupt);

#endif