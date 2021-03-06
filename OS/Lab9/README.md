# Лабораторная работа 9: Создание двух процессов.
#### Задание:
Напишите программу, которая создает подпроцесс. Этот подпроцесс должен исполнить **cat(1)** длинного файла. Родитель должен вызвать **printf(3)** и распечатать какой-либо текст. После выполнения первой части задания модифицируйте программу так, чтобы последняя строка, распечатанная родителем, выводилась после завершения порожденного процесса. Используйте **wait(2)**, **waitid(2)** или **waitpid(3)**.

#### Ход выполнения:

###### main:
Программе при запуске в качестве аргумента передается имя текстового файла, для которого порожденный процесс будет вызывать **cat**. В самом начале **main** проводится проверка на корректность введенных аргументов.

Если на этот этапе все в порядке, производится инициализация переменных ```pid_t pid;``` и ```int status;```, первая служит для хранения id процесса, вторая - статус для вызова **wait** в конце программы.

Далее выводится сообщение, которое удобно для отладки (между этим сообщением и последним сообщением от родителя будет заключен текст который является результатом работы **cat** из порожденного процесса.

Следующая строчка кода ```pid = fork();``` порождает новый процесс с помощью вызова **fork**. Для процесса-родителя она вернет pid порожденного процесса, если его создание прошло успешно, если при создании процесса произошла ошибка, то вернется -1, для самого порожденного процесса результат этого вызова будет равен 0.

> **fork** создает процесс-потомок, он отличается от родительского процесса только значениями **PID** (идентификатор процесса) и **PPID** (идентификатор родительского процесса), а также тем фактом, что счетчики использования ресурсов установлены в 0. Блокировки файлов и сигналы, ожидающие обработки, не наследуются.
 При успешном завершении родителю возвращается PID процесса-потомка, а процессу-потомку возвращается 0. При неудаче родительскому процессу возвращается -1, процесс-потомок не создается, а значение errno устанавливается должным образом.

 Далее идет проверка ```if (pid == -1)``` на успешность создания файла. Если это условие истино, то сохраняется сообщение об ошибке и программа завершается.

 Следующая проверка ```if (pid == 0)``` выполняет часть кода для порожденного процесса. Как мы разобрали выше, у процесса-ребенка значение pid будет 0, то есть это условие будет истино только для такого процесса. В этой части кода как раз содержится вызов **cat**. Этот вызов производится с помощью **execl**.

> Семейство вызовов exec загружает и запускает другую программу. Таким образом, новая программа полностью замещает текущий процесс. Новая программа начинает свое выполнение с функции main. Все файлы вызывающей программы остаются открытыми. Они также являются доступными новой программе. Используется шесть различных вариантов функций exec. Для наших задач подходит execl, где l значит list, то есть аргументы передаются в формате списка. Еще бывают в формате вектора, из файла и т.д.

Таким образом, утилита **cat**, которая находится в /bin/cat, вызывается с именем текстового файла, который мы передали нашей программе при запуске.

Последнее условие ```if (pid != 0)``` идентефицирует процесс-родитель и исполняет его часть кода. Здесь используется вызов **wait**.

> Функция wait приостанавливает выполнение текущего процесса до тех пор, пока дочерний процесс не завершится, или до появления сигнала, который либо завершает текущий процесс, либо требует вызвать функцию-обработчик. Если дочерний процесс к моменту вызова функции уже завершился (так называемый "зомби" ("zombie")), то функция немедленно возвращается. Системные ресурсы, связанные с дочерним процессом, освобождаются.

Когда функция-родитель дождется свой дочерний процесс, переданное **wait** значение приобретет статус завершения, а сама функция вернет pid завершенного процесса. Далее идет последнее сообщение от процесса-родителя, после которого завершается программа.
