#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <err.h>
#include <string.h>

#define ARRAY_SIZE 4096

int ReadFile(int* descriptor, int* stringLengthTable, int* stringPositionTable)
{
    int iterator = 0;
    int counter = 0;
    char tmp;

    while(read(*descriptor, &tmp, 1))
    {
        if (tmp == '\n')
        {
            iterator++;
            stringLengthTable[counter] = iterator;
            counter++;
            stringPositionTable[counter] = lseek(*descriptor, 0, SEEK_CUR);
            iterator = 0;
        }
        else
        {
            iterator++;
        }
    }

    return counter;
}

int FindString(int* descriptor, int* stringLengthTable, int* stringPositionTable, int totalQuantityOfStrings)
{
    int number;
    char buffer[ARRAY_SIZE];

    int value = 0;
    struct timeval tmo;
    fd_set readfds;

    while(1)
    {
        printf("You have five seconds to select string by number: ");

        fflush(stdout);

        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        tmo.tv_sec = 5;
        tmo.tv_usec = 0;

        switch (select(1, &readfds, NULL, NULL, &tmo))
        {
            case -1:
                err(1, "select");
                break;
            case 0:
                printf("\nYou don't give input\n");
                return (1);
        }

        scanf("%d",&number);

        if(number == 0)
        {
            break;
        }

        if(number < 0 || number > totalQuantityOfStrings)
        {
            printf("Incorrect string number. Next time enter a number from 1 to %d.\n", totalQuantityOfStrings);
            return -1;
        }

        lseek(*descriptor, stringPositionTable[number - 1], SEEK_SET);

        if(read(*descriptor, buffer, stringLengthTable[number - 1]))
        {
            write(STDOUT_FILENO, buffer, stringLengthTable[number - 1]);
        }
        else
        {
            printf("Cannot find line with such number.\n");
        }
    }

    return 0;
}

void SaveTables(int* stringLengthTable, int* stringPositionTable, int totalQuantityOfStrings)
{
    FILE *file = fopen("tables", "w");
    char result[64] = "";

    for (int i = 0; i != totalQuantityOfStrings; ++i)
    {
        sprintf(result, "String %d: Length = %d; Position = %d\n", i, stringLengthTable[i], stringPositionTable[i]);
        fputs(result, file);
    }
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Please, enter a name of text file as a command argument: ./<filename> <textname>\n");
        return -1;
    }

    int descriptor;
    int stringLengthTable[ARRAY_SIZE];
    int stringPositionTable[ARRAY_SIZE];

    if((descriptor = open(argv[1], O_RDONLY)) == -1)
    {
        printf("Error with file\n");
        return -1;
    }

    int totalQuantityOfStrings = ReadFile(&descriptor, stringLengthTable, stringPositionTable);
    SaveTables(stringLengthTable, stringPositionTable, totalQuantityOfStrings);
    int result = FindString(&descriptor, stringLengthTable, stringPositionTable, totalQuantityOfStrings);

    close((descriptor));

    if (result == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}