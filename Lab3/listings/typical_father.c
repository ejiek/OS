#include <unistd.h>
#include <stdio.h>

int main() {
	int pid, ppid, status;
	pid=getpid();
	ppid=getppid();
	printf("\n\n FATHER PARAM pid=%i ppid=%i\n",pid, ppid);
	if(fork()==0) execl("son","son", NULL);
	sleep(2);
}

