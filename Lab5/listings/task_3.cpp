#include <iostream>
#include <windows.h>

DWORD WINAPI threadHandler(LPVOID);

int main(int argc, char* argv[]) {
    printf("Program started\n");
    HANDLE t;
    int number = 1;
    t = CreateThread(NULL, 0, threadHandler, (LPVOID)number, 0, NULL);
    CloseHandle(t);
    number = 2;
    t = CreateThread(NULL, 0, threadHandler, (LPVOID)number, 0, NULL);
    CloseHandle(t);
    //ExitThread(0); // разкомментировать для второго варианта
    std::cout << "Program finished" << std::endl;
    Sleep(1000);
    return 0;
}
DWORD WINAPI threadHandler(LPVOID param) {
    int number = (int)param;
    for (;;) {
        Sleep(300);
        std::cout << number;
    }
    return 0;
}
