#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int skip = 2; // Skip first 2 items in the list;

int command_ls()
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir("./");
    if (dp != NULL)
    {
        int count = 0;
        while ((ep = readdir(dp)))
        {
            count++;
            if (count > skip)
            {
                puts(ep->d_name);
            }
        }
        closedir(dp);
    }
    else
        perror("Couldn't open the directory");

    return 0;
}