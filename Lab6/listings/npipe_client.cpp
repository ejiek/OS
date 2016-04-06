#include <windows.h>
#include <stdio.h>
#include <conio.h>
int main(int argc, char *argv[])
{ // Идентификатор канала Pipe
    HANDLE hNamedPipe;
    // Количество байт, переданных через канал
    DWORD cbWritten;
    // Количество байт, принятых через канал
    DWORD cbRead;
    // Буфер для передачи данных
    char szBuf[256];
    // Буфер для имени канала Pipe
    LPCWSTR szPipeName = L"\\\\.\\pipe\\$MyPipe$";
    printf("Named pipe client demo\n");
    printf("Syntax: pipec [servername]\n");
    // открываем handle нашего именованого pipe
    hNamedPipe = CreateFile(
        szPipeName, // pipe name
        GENERIC_READ | // read and write access
        GENERIC_WRITE,
        0, // no sharing
        NULL, // default security attributes
        OPEN_EXISTING, // opens existing pipe
        0, // default attributes
        NULL); // no template file
    // Если возникла ошибка, выводим ее код и
    // завершаем работу приложения
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        fprintf(stdout, "CreateFile: Error %ld\n",
            GetLastError());
        getch();
        return 0;
    }
    // Выводим сообщение о создании канала
    fprintf(stdout, "\nConnected. Type 'exit' to terminate\n");
    // Цикл обмена данными с серверным процессом
    while (1)
    { // Выводим приглашение для ввода команды
        printf("cmd>");
        // Вводим текстовую строку
        gets(szBuf);
        // Передаем введенную строку серверному процессу
        // в качестве команды
        if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1, &cbWritten, NULL)) break;
        // Получаем эту же команду обратно от сервера
        if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL))
            printf("Received back: <%s>\n", szBuf);
        // В ответ на команду "exit" завершаем цикл
        // обмена данными с серверным процессом
        if (!strcmp(szBuf, "exit"))
            break;
    }
    // Закрываем идентификатор канала
    CloseHandle(hNamedPipe);
    return 0;
}
