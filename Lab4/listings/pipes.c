#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
	printf("Max opened files per process: %d\n", sysconf(_SC_OPEN_MAX));
	int i = 0, filedes[2], local_errno;
	while(pipe(filedes) != -1){
		i++;
	}
	local_errno = errno;
	printf("errno: %d\n", local_errno);
	printf("Pipes created: %d\n", i);
	
	return 0;
}

