#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Bad program arguments. You should enter something like ./<FileName> <TextFileName>\n");
        return 1;
    }

    pid_t pid;
    int status;

    printf("=======================================================================================================\n");
    printf("First message before fork().\n");
    printf("=======================================================================================================\n");

    pid = fork();

    if (pid == -1) //if something wrong in fork
    {
        perror("problem in fork");
        return 2;
    }

    if (pid == 0) //if I am a child process
    {
        if (execl("/bin/cat", "cat", argv[1], NULL) == -1)
        {
            perror("Error execl");
            return 3;
        }
    }

    if (pid != 0) //if I am a parent process; for me this pid is child's pid
    {
        wait(&status);
        printf("=======================================================================================================\n");
        printf("Last message from parent.\n");
        printf("=======================================================================================================\n");
    }

    return 0;
}
