#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
using namespace std;

int main()
{
    char buf[2];
    unsigned long avail;
    cout << "STD INPUT HANDLE = " << GetStdHandle(STD_INPUT_HANDLE) << "\n";
    cout << "STD OUTPUT HANDLE = " << GetStdHandle(STD_OUTPUT_HANDLE) << "\n";
    unsigned long bread; //кол-во прочитанных байт
    while (1){
        PeekNamedPipe( // получаем данные из анонимного канала
            GetStdHandle(STD_INPUT_HANDLE), // идентификатор канала Pipe
            NULL, //адрес буфера для прочитанных данных(NULL -нет данных для чтения)
            NULL, //размер буфера в байтах, параметр игнорируется, если буфер NULL
            NULL, //указатель на переменную, которая получает число считанных байт //параметр = NULL, если нет данных для чтения
            &avail, //адрес переменной, в которую будет записано общее количество //байт данных,доступных в канале для чтения
            NULL); // адрес переменной, в которую будет записано количество //непрочитанных байт в данном сообщении
        if (avail)
        {
            memset(buf, '\0', sizeof(buf));
            ReadFile(
                GetStdHandle(STD_INPUT_HANDLE), //handle канала
                buf, //указатель на буфер,в который пишем считанные из //канала данные
                1, //кол-во байтов для чтения
                &bread, //указатель на переменную (кол-во считанных байт)
                NULL); // если handle не OVERLAPPED, то равен NULL
            cout << buf;
        }
    }
    return 0;
}
