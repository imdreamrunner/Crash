#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"

int IS_RUNNING = 0;

int launch_process(char **args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // 子进程。
        if (execvp(args[0], args) == -1)
        {
            perror("lanuch");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // 无法创建子进程，打印错误。
        perror("lanuch");
    }
    else
    {
        // 父进程。
        int status;
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return status;
    }
}

char *get_executable_path(char *command)
{
    int path_length = 1;
    char *path_array[MAX_PATH_NUMBER] = {"/usr/bin/"};

    for (int i = 0; i < path_length; i++)
    {
        char *path = path_array[i];
        char *executable_path = (char *)malloc(MAX_PATH_LENGTH);
        strcpy(executable_path, path);
        strcat(executable_path, "/");
        strcat(executable_path, command);
        if (access(executable_path, F_OK) != -1)
        {
            // 文件存在。
            return executable_path;
        }
        else
        {
            // 文件不存在。
            free(executable_path);
        }
    }
    return NULL;
}
