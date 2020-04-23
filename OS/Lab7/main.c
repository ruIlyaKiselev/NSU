#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <err.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define ARRAY_SIZE 4096

int ReadFile(int* descriptor, int* stringLengthTable, int* stringPositionTable)
{
    struct stat s;
    int lengthIterator = 0;
    int positionIterator = 0;
    int counter = 0;
    int bufferSize = 0;
    char* buffer = NULL;

    int status = fstat (*descriptor, &s);
    bufferSize = s.st_size;
    buffer = (char*)mmap(0, bufferSize, PROT_READ, MAP_PRIVATE, *descriptor, 0);

    for (int i = 0; i != bufferSize; ++i)
    {
        if (buffer[i] == '\n')
        {
            lengthIterator++;
            positionIterator++;
            stringLengthTable[counter] = lengthIterator;
            counter++;
            stringPositionTable[counter] = positionIterator;
            lengthIterator = 0;
        }
        else
        {
            lengthIterator++;
            positionIterator++;
        }
    }

    return counter;
}

int FindString(int* descriptor, int* stringLengthTable, int* stringPositionTable, int totalQuantityOfStrings)
{
    int number;
    char* buffer = NULL;

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

        if (scanf("%d",&number) != 1)
        {
            printf("Incorrect string number. Next time enter a number from 1 to %d.\n", totalQuantityOfStrings);
            fflush(stdin);
            continue;
        }

        if(number == 0)
        {
            break;
        }

        if(number < 0 || number > totalQuantityOfStrings)
        {
            printf("Incorrect string number. Next time enter a number from 1 to %d.\n", totalQuantityOfStrings);
            continue;
        }

        buffer = (char*)mmap(0, 1, PROT_READ, MAP_PRIVATE, *descriptor, 0);

        if (buffer != NULL)
        {
            char* result = (char*)malloc((stringPositionTable[number - 1] + stringLengthTable[number - 1] + 1) * sizeof(char*));
            for (int i = stringPositionTable[number - 1], j = 0; i != stringPositionTable[number - 1] + stringLengthTable[number - 1]; ++i, ++j)
            {
                result[j] = buffer[i];
            }

            printf("%s", result);
            free(result);
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

    return 0;
}
