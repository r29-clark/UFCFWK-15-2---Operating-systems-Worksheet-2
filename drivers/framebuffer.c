#include "framebuffer.h" //gives access to the declarations.

#define FB_ADDRESS 0x000B8000 //Define the framebuffer address
#define FB_WIDTH 80 //Define the dimensions of the screen
#define FB_HEIGHT 25

static unsigned short *fb = (unsigned short *) FB_ADDRESS; //Creates a pointer to the framebuffer.

static unsigned short cursor_x = 0;
static unsigned short cursor_y = 0;
static unsigned char text_colour = 0x0F;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = (unsigned short)c | ((unsigned short)(fg | bg << 4) << 8);
}

void fb_set_colour(unsigned char fg, unsigned char bg)
{
    text_colour = fg | (bg << 4);
}

void fb_move(unsigned short x, unsigned short y)
{
    if (x >= FB_WIDTH || y >= FB_HEIGHT)
    {
        return;
    }

    cursor_x = x;
    cursor_y = y;
}

void fb_clear()
{
    unsigned int i;

    for (i = 0; i < FB_WIDTH * FB_HEIGHT; i++)
    {
        fb_write_cell(i, ' ', 15, 0);
    }

    cursor_x = 0;
    cursor_y = 0;
}

void fb_write(char *buf)
{
    unsigned int i = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == '\n')
        {
            cursor_x = 0;
            cursor_y++;
        }
        else
        {
            fb_write_cell(cursor_y * FB_WIDTH + cursor_x,
                          buf[i], text_colour & 0x0F,
                          (text_colour >> 4) & 0x0F);

            cursor_x++;

            if (cursor_x >= FB_WIDTH)
            {
                cursor_x = 0;
                cursor_y++;
            }
        }

        if (cursor_y >= FB_HEIGHT)
        {
            cursor_y = 0;
        }

        i++;
    }
}
void fb_write_number(int number)
{
    char buffer[12];
    int i = 0;
    unsigned int value;

    if (number < 0)
    {
        fb_write("-");
        value = (unsigned int)(-number);
    }
    else
    {
        value = (unsigned int)number;
    }

    if (value == 0)
    {
        fb_write("0");
        return;
    }

    while (value > 0)
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0)
    {
        char digit[2];
        digit[0] = buffer[--i];
        digit[1] = '\0';
        fb_write(digit);
    }
}