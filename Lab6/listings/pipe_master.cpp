#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
using namespace std;
int main() { // инициализируем необходимые структуры
    STARTUPINFO si = { sizeof(si) };
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    char buf[1024];
    char t = '\n';
    HANDLE newstdread, newstdwrite;//хэндлы потоков для пайпа
    //инициализируем нужные поля SECURITY_ATTRIBUTES
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = true; //разрешаем наследование дескрипторов
    //создаем анонимный канал(создаем пайп для stdin)
    if (!CreatePipe(&newstdread, //указатель на пеpеменную типа dword, котоpая получит //хэндл конца чтения пайпа
                    &newstdwrite, //указатель на пеpеменную типа dword, котоpая получит //хэндл на конец записи пайпа
                    &sa, // указатель на структуру атрибутов без-ти
                    0))//размер буфера, исп-ся по умолч.
    {
        cout << "I can't CreatePipe";
        getch();
        return 0;
    }
    else
        cout << "\nPipe Created!\n";
    //выводим на экран дескриптор потока ввода анонимного канала
    cout << "The read HANDLE of PIPE = " << newstdread << endl;
    //обнуляем поля STARTUPINFO и задаем нужные значения
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_NORMAL;
    //подменяем стандартный дескриптор ввода дескриптором ввода канала
    si.hStdInput = newstdread;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = si.hStdOutput;
    TCHAR czCommandLine[] = L"C:\\OS\\Lab6\\pipe_slave.exe";
    if (!CreateProcess(NULL, czCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout << "Error: Can't CreateProcess";
        getch();
        CloseHandle(newstdread);
        CloseHandle(newstdwrite);
        return 0;
    }
    else
    cout << "\nProcess Created!!!\n";
    memset(buf, '\0', sizeof(buf));
    cout << buf;
    unsigned long bread;
    cout << "STD INPUT HANDLE = " << GetStdHandle(STD_INPUT_HANDLE) << endl;
    cout << "STD OUTPUT HANDLE = " << GetStdHandle(STD_OUTPUT_HANDLE) << endl;
    while (1)
    {
        memset(buf, '\0', sizeof(buf));
        *buf = (char)getch();
        cout.put(*buf);
        if (*buf == 13)
        {
            *buf = '\n';
            cout.put(*buf);
        }
            WriteFile(newstdwrite, //указатель на пишущих хэндл канала
                    buf, // указатель на буфер
                    1, //кол-во байт данных,записываемых в буфер
                    &bread,// указатель на переменную, хранящую кол-во байт, записанных в буфер
                    NULL);//т.к. 1й аргумент не был открытс флагом FILE_FLAG_OVERLAPPED
        if (*buf == 27)
            break;
    }
    TerminateProcess(pi.hProcess, 0); // завершение процесса
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(newstdread);
    CloseHandle(newstdwrite);
    system("PAUSE");
    return 0;
}
