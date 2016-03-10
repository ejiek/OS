#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	time_t rawtime;
	struct tm * timeinfo;
	int i1, i2, i3;
	int pid, ppid, status, sons_pid;
	pid=getpid();
	ppid=getppid();
	sons_pid = atoi(argv[1]);
	printf("FATHER PARAM pid=%i ppid=%i\n",pid, ppid);
	sleep(5);
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Signal SIGUSR1 sent. Current time and date: %s", asctime (timeinfo) );	
	kill(sons_pid,SIGUSR1);
	sleep(15);
}

