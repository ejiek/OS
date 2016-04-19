#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include "onecore.h" 

int main() {
	struct sched_param sp;
	int pid, ppid;
        
	pid = getpid();
	
	one_core(pid);

	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sp);
	ppid = getppid();
	printf("\nSon param: pid = %d, ppid = %d\n", pid, ppid);
	for (int i = 0; i < 10000000; ++i) {
		if (i % 1000000 == 0) printf("son\n");
	}
	return 0;
}
