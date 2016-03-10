#include <stdio.h>
#include <unistd.h>
#include "onecore.h"

int main() {
	int pid, ppid;
	if (fork() == 0) {
		pid = getpid();
		one_core(pid);
		ppid = getppid();
		printf("Son process, pid=%d, ppid=%d\n", pid, ppid);
		for (int i = 0; i < 10000000; ++i) {
			if (i % 1000000 == 0) {
				printf("son\n");
			}
		}

	} else {
		pid = getpid();
		ppid = getppid();
		one_core(pid);
		printf("Father process, pid=%d, ppid=%d\n", pid, ppid);		
		for (int i = 0; i < 10000000; ++i) {
			if (i % 1000000 == 0) {
				printf("father\n");
			}
		}
	}
	printf("Process finished %d\n", pid);
	return 0;
}
