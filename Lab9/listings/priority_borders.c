#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
	int startnice, nice, i, nnice;
	int pid, ppid, status;
	pid=getpid();
	ppid=getppid();
	startnice=getpriority(PRIO_PROCESS, pid);
	printf("Process nice priority = %d\n", nice);
	printf("\nFinding max priority: ");
	nice=getpriority(PRIO_PROCESS, pid);
	nnice = nice -1;
	while (nice != nnice) {
		nice=getpriority(PRIO_PROCESS, pid);
		nnice = nice - 1;
		setpriority(PRIO_PROCESS, pid, nnice);
		nnice=getpriority(PRIO_PROCESS, pid);
		printf(" %d,", nnice);
	}
	puts("");
	

	printf("\nFinding min priority: ");
	nnice = nice - 1;	
	while (nice != nnice){
		nice=getpriority(PRIO_PROCESS, pid);
		nnice = nice + 1;
		setpriority(PRIO_PROCESS, pid, nnice);
		nnice=getpriority(PRIO_PROCESS, pid);
		printf(" %d,", nnice);
	}
	puts("");
}

