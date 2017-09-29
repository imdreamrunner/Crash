#include <stdio.h>
#include <stdbool.h>

#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"

char input_buffer[INPUT_BUFFER_SIZE];

int return_value = 0;
bool exited = false;

/**
 * Check if the current input buffer is executing certain command
 * @param command   The command as a string.
 */
bool check_command(char *command)
{
    char *c = command;
    int position = 0;
    while (*c != 0)
    {
        if (*c != input_buffer[position])
        {
            return false;
        }
        position++;
        c++;
    }
    return input_buffer[position] == ' ' || input_buffer[position] == 0;
}

void line_loop()
{
    unsigned char c = 0;

    printf("~ > ");

    int position = 0;

    while (true)
    {
        c = getchar();

        if (c == '\n') {
            printf("\n");
            break;
        }
        else if (c == 127 || c == 8)
        {
            // Delete or backspace.
            printf("\b \b");
            position--;
        }
        else
        {
            // Other keys.
            printf("%c", c);
            input_buffer[position] = c;
            position++;
        }
    }

    input_buffer[position] = 0;

    if (check_command("exit"))
    {
        exited = true;
        return_value = 0;
    }
    else if (check_command("ls"))
    {
        command_ls();
    }
    else if (check_command("cd"))
    {
        command_cd(input_buffer + 3);
    }
}

int input_loop()
{

    while (!exited)
    {
        line_loop();
    }

    printf("Bye.\n\n");

    return return_value;
}
