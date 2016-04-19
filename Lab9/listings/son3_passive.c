#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void sig_accept() {
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Signal SIGUSR1 accepted. Current time and date: %s", asctime (timeinfo) );
}

int main() {
	int pid, ppid;
	pid=getpid();
	ppid=getppid();
	printf("SON PARAM: pid=%i ppid=%i \n", pid, ppid);
	signal(SIGUSR1, sig_accept);
	sleep(10000);
}

