#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf("main: argc = %d\n      argv = { ", argc);
    for(int i = 0; i != argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    printf("}\n");

    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) //if something wrong in fork
    {
        perror("problem in fork");
        exit(2);
    }

    if (pid == 0) //if I am a child process
    {
        if (execv(argv[1], argv + 1) == -1)
        {
            perror("Error execl");
            exit(3);
        }
    }

    if (pid != 0) //if I am a parent process; for me this pid is child's pid
    {
        wait(&status);
        printf("Return status: %d\n", WEXITSTATUS(status));
        return WEXITSTATUS(status);
    }

    return 0;
}
