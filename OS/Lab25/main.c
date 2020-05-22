#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <ctype.h>

#define PIPE_SIZE 128

void upText(char* inputText)
{
    for (int i = 0; inputText[i] != '\0'; i++)
    {
        int ch_int = (int)inputText[i];
        inputText[i] = (char)toupper(ch_int);
    }
}

int main(int argc, char* argv[])
{
    pid_t child1, child2, pid;
    int pipefd[2];
    int status;

    char *textForSending = argv[1];
    char textForReading[PIPE_SIZE];

    if (pipe(pipefd) == -1) // check pipe errors
    {
        perror("Pipe: bad pipefd");
        exit(1);
    }

    if ((child1 = fork()) == 0) // sender
    {
        printf("Child1: Start sending\n");
        close(pipefd[0]);
        write(pipefd[1], textForSending, PIPE_SIZE);
        close(pipefd[1]);
        printf("Child1: Finish sending\n");
        exit (0);
    }

    if ((child2 = fork()) == 0) // receiver
    {
        printf("Child2: Start receiving\n");
        close(pipefd[1]);
        read(pipefd[0], textForReading, PIPE_SIZE);
        close(pipefd[0]);

        upText(textForReading);

        printf("%s\n", textForReading);
        printf("Child2: Finish receiving\n");
        exit (0);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    while ((pid = wait(&status)) != -1)
    {
        if (child1 == pid)
        {
            printf("Child1: Finish\n");
        }
        else if (child2 == pid)
        {
            printf("Child2: Finish\n");
        }
    }

    return 0;
}
