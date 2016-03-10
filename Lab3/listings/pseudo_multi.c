#include <stdio.h>
#include <unistd.h>

int main() {
	int pid, ppid;
	if (fork() == 0) {
		pid = getpid();
		ppid = getppid();
		printf("Son process, pid=%d, ppid=%d\n", pid, ppid);
	} else {
		pid = getpid();
		ppid = getppid();
		printf("Father process, pid=%d, ppid=%d\n", pid, ppid);
	}
	printf("Process finished %d\n", pid);
	return 0;
}
