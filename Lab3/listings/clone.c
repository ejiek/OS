#define _GNU_SOURCE 
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int variable, fd;

int do_something(void *arg) {
	variable = 42;
	close(fd);
	printf("__Child PID: %d  PPID: %d\n", getpid(), getppid());
	sleep(5);
	//exit(0);
}

int main(int argc, char *argv[]) {
	void **child_stack;
	char tempch;

	printf("__Father PID: %d  PPID: %d\n", getpid(), getppid());
	variable = 9;
	fd = open("test.file", O_RDONLY);
	child_stack = (void *) malloc(8192);
	printf("The variable was %d\n", variable);
	
	clone(&do_something, child_stack+8192, 0, NULL);
	sleep(1);

	printf("The variable is now %d\n", variable);
	if (read(fd, &tempch, 1) < 1) {
		perror("File Read Error");
		sleep(5);
		exit(1);
	}
	printf("We can read from the file\n");
	sleep(5);
	return 0;
}
