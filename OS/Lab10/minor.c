#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    printf("minor: argc = %d\n       argv = { ", argc);
    for(int i = 0; i != argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    printf("}\n");

    if (argc > 1)
    {
        printf("Minor program: success with %s!\n", argv[1]);
        return atoi(argv[1]);
    }

    printf("Minor program: fail!\n");
    return -1;
}
