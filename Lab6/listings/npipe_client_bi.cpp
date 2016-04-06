#include <windows.h>
#include <stdio.h>
#define SIZE_OF_BUF 1000

int main(int argc, char *argv[]) {
    HANDLE hNamedPipe; // Идентификатор канала Pipe
    DWORD readbytes,writebytes; // количество байт принятых и переданных в канал
    char buf[SIZE_OF_BUF]; // Буфер для передачи данных
    printf("Client is started\n Try to use WaitNamedPipe\n"); //"ожидаем" пока освободится канал
    LPCWSTR szPipeName = L"\\\\.\\pipe\\$$MyPipe$$";
    if (!WaitNamedPipe(szPipeName, NMPWAIT_WAIT_FOREVER)) {
        printf("pipe wasn't created\n getlasterror = %d", GetLastError());
        getchar();
        return 1000;
    } // Создаем канал с процессом-сервером
    hNamedPipe = CreateFile(szPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // Если возникла ошибка, выводим ее код и
    // завершаем работу приложения
    if(hNamedPipe == INVALID_HANDLE_VALUE) {
        fprintf(stdout,"CreateFile: Error %ld\n",GetLastError());
        getchar();
        return 1001;
    } // Выводим сообщение о создании канала
    printf("successfully connected\n input message\n"); // Цикл обмена данными с серверным процессом
    while(1) { // Выводим приглашение для ввода команды printf("cmd>"); // Вводим текстовую строку
        fgets(buf,SIZE_OF_BUF,stdin); // Передаем введенную строку серверному процессу // в качестве команды
        if(!WriteFile(hNamedPipe, buf, strlen(buf) + 1, &writebytes, NULL)) {
            printf("connection refused\n");
            break;
        } // Получаем эту же команду обратно от сервера
        if(ReadFile(hNamedPipe, buf, SIZE_OF_BUF, &readbytes, NULL))
            printf("Received from server: %s\n", buf); // Если произошла ошибка, выводим ее код и // завершаем работу приложения
        else {
            fprintf(stdout,"ReadFile: Error %ld\n", GetLastError());
            getchar();
            break;
        } // В ответ на команду "exit" завершаем цикл // обмена данными с серверным процессом
        if(!strncmp(buf, "exit",4))
            break;
    } // Закрываем идентификатор канала
    CloseHandle(hNamedPipe);
    printf("client is ending\n Press any key\n");
    getchar();
    return 0;
}
