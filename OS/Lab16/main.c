#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

int main()
{
    struct termios attributes;
    struct termios tmpAttributes;

    if (tcgetattr(STDIN_FILENO, &attributes) == -1)
    {
        printf("Cannot get terminal attributes.");
        return 1;
    }

    tmpAttributes = attributes;
    attributes.c_lflag &= ~(ICANON | ECHO);
    attributes.c_cc[VMIN] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes) == -1)
    {
        printf("Unable to change terminal attributes.\n");
        return 2;
    }

    while (1)
    {
        printf("Enter some letter\n");
        char buffer;

        if (read(STDIN_FILENO, &buffer, 1) == 1)
        {
            if (isalpha(buffer))
            {
                printf("Thank you!\n");
                break;
            }
            else
            {
                printf("Bad input!\n");
            }
        }
        else
        {
            printf("Problem in reading.\n");
        }
    }

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tmpAttributes) == -1)
    {
        printf("Unable to restore default terminal attributes.\n");
        return 2;
    }

    return 0;
}