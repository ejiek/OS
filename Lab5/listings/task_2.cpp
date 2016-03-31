#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <iostream>
#define MAX_LEN 200

int main(int argc, char* argv[]) {
    const char* frd = "C:\\temp.txt";
    FILE *f=fopen(frd,"r");
    if(f==NULL) {
        std::cout << "Coudn't open file" << std::endl;
        system("pause");
        return 1;
    }
    for (int i = 0; i<2; i++) {
        wchar_t* execString = (wchar_t*)calloc(MAX_LEN, sizeof(wchar_t));
        //выделение памяти
        fgetws(execString, MAX_LEN, f);
        // чтение строки из файла
        execString[wcslen(execString) - 1] = '\0';
        STARTUPINFO startupInfo;
        ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo;
        std::wcout << execString;
        std::cout << std::endl;
        if( !CreateProcess(NULL, execString, NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo) ) {
            std::cout << "Error creating process: " << GetLastError() << std::endl;
            return -1;
        } else
            printf("Process successfully created!\n");
        free(execString);
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }
    return 0;
}
