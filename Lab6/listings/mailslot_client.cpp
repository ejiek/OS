#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main(int argc, char *argv[])
{
    // Идентификатор канала Mailslot
    HANDLE hMailslot;
    // Буфер для имени канала Mailslot
    LPCWSTR   szMailslotName = L"\\\\.\\mailslot\\$Channel$";
    // Буфер для передачи данных через канал
    char   szBuf[512];
    // Количество байт, переданных через канал
    DWORD  cbWritten;
    // Создаем канал с процессом MSLOTS
    hMailslot = CreateFile(
        szMailslotName, GENERIC_WRITE,
        FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    // Если возникла ошибка, выводим ее код и
    // завершаем работу приложения
    if (hMailslot == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "CreateFile: Error %ld\n",
            GetLastError());
        getch();
        return 0;
    }

    // Выводим сообщение о создании канала
    fprintf(stdout, "\nConnected. Type 'exit' to terminate\n");

    // Цикл посылки команд через канал
    while (1)
    {
        // Выводим приглашение для ввода команды
        printf("cmd>");

        // Вводим текстовую строку
        gets(szBuf);

        // Передаем введенную строку серверному процессу
        // в качестве команды
        if (!WriteFile(hMailslot, szBuf, strlen(szBuf) + 1,
            &cbWritten, NULL))
            break;

        // В ответ на команду "exit" завершаем цикл
        // обмена данными с серверным процессом
        if (!strcmp(szBuf, "exit"))
            break;
    }

    // Закрываем идентификатор канала
    CloseHandle(hMailslot);
    return 0;
}
