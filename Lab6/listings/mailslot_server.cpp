#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main() {
    // Код возврата из функций
    BOOL   fReturnCode;
    // Размер сообщения в байтах
    DWORD  cbMessages;
    // Количество сообщений в канале Mailslot
    DWORD  cbMsgNumber;
    // Идентификатор канала Mailslot
    HANDLE hMailslot;
    // Имя создаваемого канала Mailslot
    LPCWSTR  lpszMailslotName = L"\\\\.\\mailslot\\$Channel$";
    // Буфер для передачи данных через канал
    char   szBuf[512];
    // Количество байт данных, принятых через канал
    DWORD  cbRead;

    // Создаем канал Mailslot, имеющий имя lpszMailslotName
    hMailslot = CreateMailslot(
        lpszMailslotName, 0,
        MAILSLOT_WAIT_FOREVER, NULL);

    // Если возникла ошибка, выводим ее код и зваершаем
    // работу приложения
    if (hMailslot == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "CreateMailslot: Error %ld\n",
            GetLastError());
        getch();
        return 0;
    }

    // Выводим сообщение о создании канала
    fprintf(stdout, "Mailslot created\n");

    // Цикл получения команд через канал
    while (1)
    {
        // Определяем состояние канала Mailslot
        fReturnCode = GetMailslotInfo(
            hMailslot, NULL, &cbMessages,
            &cbMsgNumber, NULL);

        if (!fReturnCode)
        {
            fprintf(stdout, "GetMailslotInfo: Error %ld\n",
                GetLastError());
            getch();
            break;
        }

        // Если в канале есть Mailslot сообщения,
        // читаем первое из них и выводим на экран
        if (cbMsgNumber != 0)
        {
            if (ReadFile(hMailslot, szBuf, 512, &cbRead, NULL))
            {
                // Выводим принятую строку на консоль
                printf("Received: <%s>\n", szBuf);

                // Если пришла команда "exit",
                // завершаем работу приложения
                if (!strcmp(szBuf, "exit"))
                    break;
            }
            else
            {
                fprintf(stdout, "ReadFile: Error %ld\n",
                    GetLastError());
                getch();
                break;
            }
        }

        // Выполняем задержку на  500 миллисекунд
        Sleep(500);
    }

    // Перед завершением приложения закрываем
    // идентификатор канала Mailslot
    CloseHandle(hMailslot);
    return 0;
}
