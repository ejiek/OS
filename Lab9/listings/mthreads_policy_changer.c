#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct sched_param sp;

void main(int argc, char *argv[]) {
	sp.sched_priority = sched_get_priority_min(SCHED_FIFO);
	sched_setscheduler(atoi(argv[1]), SCHED_FIFO, &sp);
}

