#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define BUF_SIZE 256
TCHAR szName[] = TEXT("MyFileMappingObject");
TCHAR szMsg[] = TEXT("Message from first process");
HANDLE WINAPI mutex;
int main()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;
    mutex = CreateMutex(NULL, false, TEXT("SyncMutex"));
    // create a memory, wicth two proccess will be working
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, // использование файла подкачки
        NULL, // защита по умолчанию
        PAGE_READWRITE, // доступ к чтению/записи
        0, // макс. размер объекта
        BUF_SIZE, // размер буфера
        szName); // имя отраженного в памяти объекта
    if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE)
    {
        printf("Не может создать отраженный в памяти объект (%ld).\n",
            GetLastError());
        return 1;
    }
    pBuf = (LPTSTR)MapViewOfFile(hMapFile, //дескриптор проецируемого в памяти объекта
        FILE_MAP_ALL_ACCESS, // разрешение чтения/записи(режим доступа)
        0, //Старшее слово смещения файла, где начинается отображение
        0, //Младшее слово смещения файла, где начинается отображение
        BUF_SIZE); //Число отображаемых байтов файла
    if (pBuf == NULL)
    {
        printf("Представление проецированного файла невозможно (%ld).\n",
            GetLastError());
        return 2;
    }
    int i = 0;
    while (true)
    {
        i = rand();
        itoa(i, (char *)szMsg, 10);
        WaitForSingleObject(mutex, INFINITE);
        CopyMemory((PVOID)pBuf, szMsg, sizeof(szMsg));
        printf("write message: %s\n", (char *)pBuf);
        Sleep(1000); //необходимо только для отладки - для удобства представления и анализа //результатов
        ReleaseMutex(mutex);
    }
    // освобождение памяти и закрытие описателя handle
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(mutex);
}
