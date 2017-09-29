#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"

char input_buffer[INPUT_BUFFER_SIZE];
char current_command[INPUT_BUFFER_SIZE];

int return_value = 0;
bool exited = false;
int position = 0;

/** 
 * Process current line in the buffer.
 */
void process_input()
{
    // Set current command.
    int p = 0;
    char *c = input_buffer;
    while (*c != 0 && *c != ' ')
    {
        current_command[p] = *c;
        c++;
        p++;
    }
    current_command[p] = 0;
}

/**
 * Check if the current input buffer is executing certain command
 * @param command   The command as a string.
 */
bool check_command(char *command)
{
    return strcmp(current_command, command) == 0;
}

void restart_input()
{
    position = 0;
    printf(" > ");
}

void line_loop()
{
    unsigned char c = 0;

    restart_input();

    while (true)
    {
        c = getchar();

        if (c == '\n')
        {
            printf("\n");
            break;
        }
        else if (c == 127 || c == 8)
        {
            if (position > 0)
            {
                // Delete or backspace.
                printf("\b \b");
                position--;
            }
        }
        else if (c == '\t')
        {
            // This is a tab.
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
    process_input();

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
    else if (check_command("clear"))
    {
        printf("\e[1;1H\e[2J");
    }
    else
    {
        if (strlen(current_command)) {
            printf("Command \"%s\" is not found.\n", current_command);
        }
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

void sigint_handler()
{
    printf("\nPlease use \"exit\" command to exit the shell.\n");

    restart_input();
}