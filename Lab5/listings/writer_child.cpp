#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	HANDLE hFile;
	char DataBuffer[] = "I'm your son\n";
	DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
	DWORD dwBytesWritten = 0;
	hFile = (HANDLE)atoi(argv[1]);
	BOOL bErrorFlag = WriteFile(
		hFile,           // open file handle
		DataBuffer,      // start of data to write
		dwBytesToWrite,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);           // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		printf("Terminal failure: Unable to write to file. Error #%d\n", GetLastError());
	}
	if (CloseHandle(hFile) == 0)
		printf("Error %d\n", GetLastError());
	system("pause");
	return 0;
}
