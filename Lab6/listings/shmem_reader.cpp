#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define BUF_SIZE 256
#define TIME 15 // number of reading operation in this process
TCHAR szName[] = TEXT("MyFileMappingObject");
HANDLE WINAPI mutex;
int main()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;
    mutex = OpenMutex(
        MUTEX_ALL_ACCESS, // request full access
        FALSE, // handle not inheritable
        TEXT("SyncMutex")); // object name
    if (mutex == NULL)
        printf("OpenMutex error: %ld\n", GetLastError());
    else printf("OpenMutex successfully opened the mutex.\n");
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS, // доступ к чтению/записи
        FALSE, // имя не наследуется
        szName); // имя "проецируемого " объекта
    if (hMapFile == NULL)
    {
        printf("Невозможно открыть объект проекция файла (%ld).\n", GetLastError());
        return 1;
    }
    pBuf = (LPTSTR)MapViewOfFile(hMapFile, // дескриптор "проецируемого" объекта
        FILE_MAP_ALL_ACCESS, // разрешение чтения/записи
        0,
        0,
        BUF_SIZE);
    if (pBuf == NULL)
    {
        printf("Представление проецированного файла (%ld) невозможно .\n", GetLastError());
        return 2;
    }
    for (int i = 0; i<TIME; i++)
    {
        WaitForSingleObject(mutex, INFINITE);
        printf("read message: %s\n", (char *)pBuf);
        ReleaseMutex(mutex);
    }
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 0;
}
