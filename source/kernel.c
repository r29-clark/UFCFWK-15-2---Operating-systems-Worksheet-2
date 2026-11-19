#include "../drivers/framebuffer.h"

void kernel_main()
{
    fb_clear();

    fb_set_colour(15, 0);
    fb_write("Hello from the framebuffer!");

    fb_move(0, 2);
    fb_set_colour(14, 0);
    fb_write("Framebuffer driver is working!");

    fb_move(0, 4);
    fb_set_colour(10, 0);
    fb_write("Number test: ");
    fb_write_number(12345);

    fb_move(0, 5);
    fb_set_colour(12, 0);
    fb_write("Negative number test: ");
    fb_write_number(-123);
}