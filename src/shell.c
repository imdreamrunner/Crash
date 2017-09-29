#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "shell.h"
#include "commands/ls.h"
#include "commands/cd.h"

char input_buffer[INPUT_BUFFER_SIZE];
char current_command[INPUT_BUFFER_SIZE];
char current_working_directory[MAX_PATH_LENGTH];

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

void get_current_folder(char *output)
{
    int last_slash = 0;
    int cwd_length = strlen(current_working_directory);
    for (int i = 0; i < cwd_length; i++)
    {
        if (current_working_directory[i] == '/')
        {
            last_slash = i;
        }
    }
    for (int i = last_slash + 1; i < cwd_length; i++)
    {
        output[i - last_slash - 1] = current_working_directory[i];
    }
    output[cwd_length - last_slash] = 0;
}

void restart_input()
{
    position = 0;
    getcwd(current_working_directory, sizeof(current_working_directory));
    char current_folder[MAX_PATH_LENGTH];
    get_current_folder(current_folder);
    printf("%s > ", current_folder);
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
    else if (check_command("pwd"))
    {
        printf("%s\n", current_working_directory);
    }
    else if (check_command("clear"))
    {
        printf("\e[1;1H\e[2J");
    }
    else
    {
        if (strlen(current_command))
        {
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