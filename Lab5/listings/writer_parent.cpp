#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
int main(void){
	TCHAR cmdLine[100];
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;

	HANDLE hFile;
	char DataBuffer[] = "I'm your father!\n";
	DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
	DWORD dwBytesWritten = 0;
	printf("\n");

	system("pause");
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	hFile = CreateFile(TEXT("file.txt"),				// name of the write
		GENERIC_WRITE,					// open for writing
		0,							// do not share
		&sa,							// default security
		CREATE_NEW,					// create new file only
		FILE_ATTRIBUTE_NORMAL,			// normal file
		NULL);						// no attr. template
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error #%d", GetLastError());
		return -1;
	}
	BOOL bErrorFlag = WriteFile(
		hFile,           		// open file handle
		DataBuffer,      	// start of data to write
		dwBytesToWrite,  	// number of bytes to write
		&dwBytesWritten, 	// number of bytes that were written
		NULL);           	// no overlapped structure

	if (FALSE == bErrorFlag)
	{
		printf("Terminal failure: Unable to write to file. Error #%d\n", GetLastError());
	}
	wsprintf(cmdLine, TEXT("D:\ConsoleApplication1.exe %d"), (int)hFile);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL,cmdLine,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,	NULL,&si,&pi)){
		printf("Error creating process: %d\n", GetLastError());
		system("pause");
		return GetLastError();
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	system("pause");
	CloseHandle(hFile);
	return 0;
}
