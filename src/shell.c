#include <stdio.h>
#include <stdbool.h>

#include "shell.h"
#include "commands/ls.h"

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
    return true;
}

void line_loop()
{
    unsigned char c;

    printf("~ > ");

    int position = 0;

    do
    {
        c = getchar();

        if (c == 127 || c == 8)
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

    } while (c != '\n');

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
