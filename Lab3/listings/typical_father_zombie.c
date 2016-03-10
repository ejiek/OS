#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int pid, ppid, status;
	pid=getpid();
	ppid=getppid();
	printf("\n\n FATHER PARAM pid=%i ppid=%i\n",pid, ppid);
	if(fork()==0) execl("son","son", NULL);
	sleep(4);
	system("ps fx");
}

