#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "onecore.h"

int main() {
	struct sched_param sp;
	struct timespec tmin;
	int pid, ppid, status;

	pid = getpid();

	one_core(pid);

	sp.sched_priority = sched_get_priority_max(SCHED_RR);
	sched_setscheduler(0, SCHED_RR, &sp);
	sched_rr_get_interval(0, &tmin);
	printf("time left %d\n", tmin.tv_sec + tmin.tv_nsec/1000000);
	ppid = getppid();
	printf("\nFATHER PARAM: pid = %d, ppid = %d\n\n", pid, ppid);
	if (fork() == 0) execl("son", "son", NULL);
	for (int i = 0; i < 10000000; ++i) {
		if (i % 1000000 == 0) printf("father\n");
	}
	wait(&status);
	return 0;
}
