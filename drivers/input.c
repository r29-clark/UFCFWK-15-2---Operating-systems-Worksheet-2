#include "input.h"

int readline(char *buffer, int size)
{
    int index = 0;

    if (buffer == 0 || size <= 0)
        return -1;

    while (1) {
        int c = getc();

        if (c == -1) {
            __asm__ volatile ("pause");
            continue;
        }

        if (c == '\n') {
            buffer[index] = '\0';
            return index;
        }

        if (c == '\b') {
            if (index > 0)
                index--;
            continue;
        }

        if (index < size - 1) {
            buffer[index++] = (char)c;
        }
    }
}