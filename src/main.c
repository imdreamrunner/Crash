#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdio.h>

#include "shell.h"

// This is the entry point of the shell.
int main()
{
    struct termios old_tio, new_tio;

    // Disable output buffer.
    setbuf(stdout, NULL);

    // Get old terminal IO settings.
    tcgetattr(STDIN_FILENO, &old_tio);

    // Create new settings.
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    // Apply new settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // Hook Ctrl + C event.
    signal(SIGINT, sigint_handler);

    // Start shell input looping.
    int return_value = input_loop();

    // Restore old settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return return_value;
}
