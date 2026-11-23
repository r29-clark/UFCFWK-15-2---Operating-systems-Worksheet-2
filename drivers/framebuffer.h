#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

void fb_clear();
void fb_move(unsigned short x, unsigned short y);
void fb_write(char *buf);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_set_colour(unsigned char fg, unsigned char bg);
void fb_write_number(int number);
void fb_write_char(char c);
void fb_newline(void);
void fb_backspace(void);
#endif