#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
	int pid, ppid;
	pid=getpid();
	ppid=getppid();
	printf("\n SON PARAM: pid=%i ppid=%i \n", pid, ppid);
	signal(SIGUSR1, SIG_DFL);
	sleep(15);
}

