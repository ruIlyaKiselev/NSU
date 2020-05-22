
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>

size_t soundCounter = 0;

void handlerSIGINT(int sig)
{
    if (sig == SIGINT)
    {
        printf("\a"); // play sound
        fflush(stdout);
        ++soundCounter;
    }
}

void handlerSIGQUIT(int sig)
{
    if (sig == SIGQUIT)
    {
        printf("Count of sounds: %d\n", soundCounter);
        exit(0);
    }
}

int main()
{
    sigset(SIGINT, handlerSIGINT);
    sigset(SIGQUIT, handlerSIGQUIT);

    while (1)
    {

    }

    return 0;
}