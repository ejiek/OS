#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <iostream>
#define DEF_BUFLEN 200

int main(int argc, char* argv[]) {

    printf("Program started\n");
    if (argc < 2){
        std::cout << "Input name of configuration file" << std::endl;
        exit(-1);
    }
    const char* frd = argv[1];
    FILE *f = fopen(frd, "r");
    if(f==NULL){
        std::cout << "error opening file " << argv[1] << std::endl;
        exit(-2);
    }
    wchar_t commandLine[DEF_BUFLEN];
    for (int i = 0; i<2; i++) {
        wchar_t* execString = (wchar_t*)calloc(DEF_BUFLEN, sizeof(wchar_t));
        //выделение памяти
        fgetws(execString, DEF_BUFLEN, f);
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
