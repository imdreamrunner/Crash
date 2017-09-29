#include <stdio.h>
#include <unistd.h>

int command_cd(char *path)
{
    int error = chdir(path);
    if (error) {
        printf("Unable to change directory to \"%s\".\n", path);
    }
    return error;
}
