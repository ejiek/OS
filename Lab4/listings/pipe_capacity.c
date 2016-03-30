#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
	int i = 0, filedes[2], buflen;
	if(pipe(filedes) < 0) {
		printf("Father: can't create pipe\n");
		exit(1);
	}
	printf("pipe is successfully created\n");
	
	char buf[2];
	strcpy(buf, " ");
	buflen = sizeof(buf);
	while(write(filedes[1],buf,buflen) != -1){
		i++;
		if(i*buflen > 65530)printf("%d\n", i*buflen);
	}
	return 0;
}

