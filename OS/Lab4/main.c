//#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_SIZE 4096
//#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <crtdbg.h>

struct LinkedList
{
    char* data;
    struct LinkedList* nextNode;
};

struct LinkedList* NewNode(char* inputData) // функция создания нового узла
{
    /*
     * переменная, в которой хранится размер строки без учета \n.
     */
    size_t dataLength = strlen(inputData);

    /*
     * заготовка для возвращения из функции; не факт, что она будет успешно создана.
     */
    struct LinkedList* result = (struct LinkedList*)malloc(sizeof(struct LinkedList));

    /*
     * проверка случая, когда что-то пошло не так и новая нода не создалась (имеет nullptr).
     */
    if (result == NULL)
    {
        printf("I can't create a node\n");
        return NULL;
    }

    /*
     * выделяем память под текст для текущего узла;
     * важно отметить, что strlen возвращает размер строки без учета \n,
     * поэтому в malloc надо добавить для него еще один байт.
     */
    result->data = (char*)malloc((dataLength + 1) * sizeof(char));

    /*
     * проверка случая, когда что-то пошло не так и не удается выделить память под текст (имеет nullptr).
     */
    if (result->data == NULL)
    {
        printf("I can't create a text in new node\n");
        return NULL;
    }

    /*
     * завершающий этап создания узла - здесь в выделенную память копируется текст, ранее введенный через
     * консоль и в качестве последнего символа ставится \n чтобы это была правильная C-style строка; в качестве
     * следующего узла в списке ставится nullptr т.к. данный узел пока последний.
     */
    strncpy(result->data, inputData, dataLength);
    result->data[dataLength] = '\0';
    result->nextNode = NULL;

    return result;
}

void ReadText(struct LinkedList** head) // функция чтения текста и добавления его
{
    struct LinkedList* iterator = *head;
    /*
     * здесь будет временно храниться введенный с консоли текст
     */
    char buffer[ARRAY_SIZE];

    /*
     * пока есть поток данных и он не равен точке (по условию задачи), выполняется цикл
     * для каждого фрагмента текста добавляется новый узел с этим текстом
     */
    while (fgets(buffer, ARRAY_SIZE, stdin) != NULL)
    {
        if (buffer[0] == '.')
        {
            break;
        }

        iterator->nextNode = NewNode(buffer);

        /*
         * в этой проверке указан случай, что при выделении памяти что-то пошло не так (рассмотрено выше)
         */
        if (iterator->nextNode != NULL)
        {
            iterator = iterator->nextNode;
        }
        else
        {
            break;
        }
    }
}

void PrintList(struct LinkedList** head) // функция вывода списка
{
    struct LinkedList* iterator = (*head)->nextNode;

    while (iterator != NULL)
    {
        printf("%s", iterator->data);
        iterator = iterator->nextNode;
    }
}

void FreeNodes(struct LinkedList** head) // функция очистки списка
{
    /*
     * эта нода будет использоваться для обхода списка
     */
    struct LinkedList* tmp = NULL;

    /*
     * идем по списку с помощью итератора до тех пор, пока не достигли конца списка (пока nextNode != 0).
     * это не настоящий итератор, просто я его так назвал
     */
    while ((*head)->nextNode != NULL)
    {
        tmp = (*head)->nextNode;
        free((*head)->data);
        free((*head));
        (*head) = tmp;
    }
}

int main()
{
    /*
     * head пригодится для обхода всего списка при выводе;
     * iterator используется как текущий элемент при добавлении новых узлов и обходе.
     */
    struct LinkedList* head;

    /*
     * инициализация списка, по умолчанию все пустое
     */
    head = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    head->data = NULL;
    head->nextNode = NULL;

    ReadText(&head);
    PrintList(&head);
    FreeNodes(&head);
    free(head->data);
    free(head);

    //_CrtDumpMemoryLeaks();
    return 0;
}
