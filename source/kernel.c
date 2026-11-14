#include "../drivers/framebuffer.h"

void kernel_main()
{
    fb_clear();

    fb_set_colour(15, 0);
    fb_write("Hello from the framebuffer!");

    fb_move(0, 2);
    fb_set_colour(14, 0);
    fb_write("Framebuffer driver is working!");
}