#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell.h"
#include "color.h"
#include "launch.h"
#include "commands/ls.h"
#include "commands/cd.h"

// 输入缓存，当前行输入的内容。
char input_buffer[INPUT_BUFFER_SIZE];
// 当前命令，第一个输入的内容。
char current_command[INPUT_BUFFER_SIZE];
// 当前工作路径
char current_working_directory[MAX_PATH_LENGTH];
// 当前参数
char *current_arguments[MAX_ARGUMENT_NUMBER];

int return_value = 0;
bool exited = false;
int position = 0;

/** 
 * Process current line in the buffer.
 */
void process_input()
{
    int argument_index = 0;
    int current_position = 0;
    current_arguments[0] = malloc(MAX_PATH_LENGTH);
    for (int i = 0; i < strlen(input_buffer); i++)
    {
        if (input_buffer[i] == ' ')
        {
            // Terminate current argument.
            current_arguments[argument_index][current_position] = 0;
            argument_index++;
            current_position = 0;
            current_arguments[argument_index] = malloc(MAX_PATH_LENGTH);
            continue;
        }
        current_arguments[argument_index][current_position] = input_buffer[i];
        current_position++;
    }
    // Terminate current argument.
    current_arguments[argument_index][current_position] = 0;
    current_arguments[argument_index + 1] = 0;

    strcpy(current_command, current_arguments[0]);
}

void free_input() {

}

/**
 * Check if the current input buffer is executing certain command
 * @param command   The command as a string.
 */
bool check_command(char *command)
{
    return strcmp(current_command, command) == 0;
}

bool exist_in_path()
{
    return get_executable_path(current_command) != NULL;
}

bool start_with(char *prefix)
{
    return strncmp(prefix, current_command, strlen(prefix)) == 0;
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
    output[cwd_length - last_slash - 1] = 0;
}

void restart_input()
{
    position = 0;
    getcwd(current_working_directory, sizeof(current_working_directory));
    char current_folder[MAX_PATH_LENGTH];
    get_current_folder(current_folder);
    printf(ANSI_COLOR_BLUE "%s " ANSI_COLOR_YELLOW "> " ANSI_COLOR_RESET, current_folder);
}

/**
 * 处理按键。
 * @param c   按键代码。
 */
bool process_key(unsigned char c)
{
    if (c == '\n') // Newline 回车
    {
        printf("\n");
        return true;
    }
    else if (c == 127 || c == 8) // Delete or Backspace
    {
        if (position > 0)
        {
            printf("\b \b");
            position--;
        }
    }
    else if (c == '\t') // Tab
    {
    }
    else if (c == 27) // Escape
    {
        char next = getchar();
        if (next == 91)
        {
            switch (getchar())
            {
            case 65:
                break;
            }
        }
        else
        {
            return process_key(next);
        }
    }
    else // 其它按键。
    {
        printf("%c", c);
        // 加进输入缓存里。
        input_buffer[position] = c;
        position++;
    }
    return process_key(getchar());
}

void line_loop()
{
    restart_input();

    while (true)
    {
        if (process_key(getchar()))
        {
            break;
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
        command_cd(current_arguments[1]);
    }
    else if (check_command("pwd"))
    {
        printf("%s\n", current_working_directory);
    }
    else if (check_command("clear"))
    {
        printf("\e[1;1H\e[2J");
    }
    else if (exist_in_path() || start_with("./"))
    {
        if (!start_with("./")) {
            // 替换完整路径。
            current_arguments[0] = get_executable_path(current_command);
        }
        launch_process(current_arguments);
    }
    else
    {
        if (strlen(current_command))
        {
            printf("Command \"%s\" is not found.\n", current_command);
        }
    }
    free_input();
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
    if (is_child_process_running()) {

    } else {
        printf("\nPlease use \"exit\" command to exit the shell.\n");
        printf("\n");
        restart_input();
    }
}