#include <stdio.h>
#include <unistd.h>

int command_cd(char *path)
{
    return chdir(path);
}
