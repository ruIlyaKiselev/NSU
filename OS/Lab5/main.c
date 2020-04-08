#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define ARRAY_SIZE 4096

int ReadFile(int* descriptor, int* stringLengthTable, int* stringPositionTable)
{
    int lengthIterator = 0;
    int positionIterator = 0;
    int counter = 0;
    int bufferSize = 0;
    char buffer[ARRAY_SIZE];

    while(bufferSize = read(*descriptor, buffer, ARRAY_SIZE))
    {
        if (bufferSize < 0)
        {
            break;
        }

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
    }

    return counter;
}

int FindString(int* descriptor, int* stringLengthTable, int* stringPositionTable, int totalQuantityOfStrings)
{
    int number;
    char buffer[ARRAY_SIZE];

    while(1)
    {
        printf("Select string by number: ");

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

        lseek(*descriptor, stringPositionTable[number - 1], SEEK_SET);

        if(read(*descriptor, buffer, stringLengthTable[number - 1]))
        {
            write(STDOUT_FILENO, buffer, stringLengthTable[number - 1]);
        }
        else
        {
            printf("Cannot find line with such number.\n");
            continue;
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