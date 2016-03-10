#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_accept() {
	printf("\nmy signal SIGUSR1 accepted*\n");
}

int main() {
	int pid, ppid;
	pid=getpid();
	ppid=getppid();
	printf("\n SON PARAM: pid=%i ppid=%i \n", pid, ppid);
	signal(SIGUSR1, sig_accept);
	sleep(15);
}

