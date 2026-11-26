
#include "terminal.h"
#include "framebuffer.h"
#include "input.h"

static int str_equal(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b)
            return 0;

        a++;
        b++;
    }

    return *a == *b;
}

static void command_help(char *args)
{
    (void)args;
    fb_write("Available commands:");
    fb_newline();
    fb_write("help - Show available commands");
    fb_newline();
    fb_write("echo - Display text");
    fb_newline();
    fb_write("clear - Clear the screen");
    fb_newline();
    fb_write("version - Show OS version");
    fb_newline();
}

static void command_echo(char *args)
{
    fb_write(args);
    fb_newline();
}

static void command_clear(char *args)
{
    (void)args;
    fb_clear();
}

static void command_version(char *args)
{
    (void)args;
    fb_write("MyOS version 1.0");
    fb_newline();
}

struct command {
    const char *name;
    void (*function)(char *args);
};

static struct command commands[] = {
    {"help", command_help},
    {"echo", command_echo},
    {"clear", command_clear},
    {"version", command_version}
};

static void process_command(char *line)
{
    char *command = line;
    char *args = line;

    while (*args && *args != ' ')
        args++;

    if (*args == ' ') {
        *args = '\0';
        args++;
        while (*args == ' ')
            args++;
    }

    if (command[0] == '\0')
        return;

    for (unsigned int i = 0;
         i < sizeof(commands) / sizeof(commands[0]);
         i++)
    {
        if (str_equal(command, commands[i].name)) {
            commands[i].function(args);
            return;
        }
    }

    fb_write("Unknown command: ");
    fb_write(command);
    fb_newline();
}

void terminal_run(void)
{
    char line[256];

    fb_write("Welcome to MyOS!");
    fb_newline();
    fb_write("Type help for available commands.");
    fb_newline();

    while (1) {
        fb_write("myos> ");

        int length = readline(line, sizeof(line));

        if (length >= 0)
            process_command(line);
    }
}
