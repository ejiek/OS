#include <windows.h>
#include <stdio.h>
#include <conio.h>
int main()
{
    // Флаг успешного создания канала
    BOOL fConnected;
    // Идентификатор канала Pipe
    HANDLE hNamedPipe;
    // Имя создаваемого канала Pipe
    LPCWSTR lpszPipeName = L"\\\\.\\pipe\\$MyPipe$";
    // Буфер для передачи данных через канал
    char szBuf[512];
    // Количество байт данных, принятых через канал
    DWORD cbRead;
    // Количество байт данных, переданных через канал
    DWORD cbWritten;
    printf("Named pipe was creating \n");
    // Создаем канал Pipe, имеющий имя lpszPipeName
    hNamedPipe = CreateNamedPipe(
        lpszPipeName, //имя канала
        PIPE_ACCESS_DUPLEX, // режим котрытия канала - двунаправленный
        //параметры режима pipe:
        PIPE_TYPE_MESSAGE //Данные записываются в канал в виде потока сообщений
        | PIPE_READMODE_MESSAGE //Данные считываются в виде потока сообщений
        | PIPE_WAIT,// блокирующий режим
        PIPE_UNLIMITED_INSTANCES, //неограниченное кол-во "подключений" к каналу
        512, 512, //кол-во байт входного и вызодного буферов
        0, //тайм-аут в 50 миллисекунд (по умолчанию)
        NULL); // дескриптор безопасности по умолчанию
    // Если возникла ошибка, выводим ее код и
    // завершаем работу приложения
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "CreateNamedPipe: Error %ld\n",
            GetLastError());
        getch();
        return 0;
    }
    // Выводим сообщение о начале процесса создания канала
    fprintf(stdout, "Waiting for connect...\n");
    // Ожидаем соединения со стороны клиента
    fConnected = ConnectNamedPipe(hNamedPipe, // имя канала
        NULL); // overlapped=null
    // При возникновении ошибки выводим ее код
    if (!fConnected)
    {
        switch (GetLastError())
        {
        case ERROR_NO_DATA:
            fprintf(stdout, "ConnectNamedPipe: ERROR_NO_DATA");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        case ERROR_PIPE_CONNECTED:
            fprintf(stdout,
                "ConnectNamedPipe: ERROR_PIPE_CONNECTED");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        case ERROR_PIPE_LISTENING:
            fprintf(stdout,
                "ConnectNamedPipe: ERROR_PIPE_LISTENING");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        case ERROR_CALL_NOT_IMPLEMENTED:
            fprintf(stdout,
                "ConnectNamedPipe: ERROR_CALL_NOT_IMPLEMENTED");
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        default:
            fprintf(stdout, "ConnectNamedPipe: Error %ld\n",
                GetLastError());
            getch();
            CloseHandle(hNamedPipe);
            return 0;
            break;
        }
        CloseHandle(hNamedPipe);
        getch();
        return 0;
    } // Выводим сообщение об успешном создании канала
    fprintf(stdout, "\nConnected. Waiting for command...\n");
    // Цикл получения команд из канала
    while (1)
    {
        if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL))
        {
            // Выводим принятую команду на консоль
            printf("Received: <%s>\n", szBuf);
            // Если пришла команда "exit",
            // завершаем работу приложения
            if (!strcmp(szBuf, "exit"))
                break;
            // отправляем эхо-ответ
            if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1, &cbWritten, NULL)) break;
        }
        else
        {
            getch();
            break;
        }
    }
    CloseHandle(hNamedPipe);
    return 0;
}
