#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int i1, i2, i3;
	int pid, ppid, status;
	pid=getpid();
	ppid=getppid();
	printf("\n\n FATHER PARAM pid=%i ppid-%i\n",pid, ppid);
	if((i1=fork())==0) execl("son1","son1", NULL);
	if((i2=fork())==0) execl("son2","son2", NULL);
	if((i3=fork())==0) execl("son3","son3", NULL);
	sleep(5);       
	kill(i1,SIGUSR1);
	kill(i2,SIGUSR1);
	kill(i3,SIGUSR1);
	sleep(7);    
	wait(&status);
}

