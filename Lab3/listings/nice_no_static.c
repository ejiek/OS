#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

struct sched_param sp;
int pid;

void prnt_policy(int thread_number){
	char info[80];
	char author[20];
	switch (sched_getscheduler (0)) 
	{ 
		case SCHED_FIFO:
		strcpy(info,"SCHED__FIFO"); break;
		case SCHED_RR:
		strcpy(info,"SCHED_RR"); break;
		case SCHED_OTHER:
		strcpy(info,"SCHED_OTHER"); break; case -1:
		perror ("SCHED_GETSCHEDULER"); break; default:
		strcpy(info,"Unknown policy");
	}

	switch (thread_number){
		case 0:
		strcpy(author, "Main thread"); break; default:
		sprintf(author, "Thread %d", thread_number);
	}

	printf("%s policy: %s\n", author, info);
}


void prnt_priority(int thread_number){
	sched_getparam(0, &sp);
	printf("Static Priority: %d\n", sp.sched_priority);
	printf("Dynamic Priority:: %d\n", getpriority(PRIO_PROCESS, pid));
}


void main() {
	pid = getpid();

	puts("Main thread: switch policy to SCHED_OTHER with max priority");
	sp.sched_priority = sched_get_priority_max(SCHED_OTHER);
	sched_setscheduler(0, SCHED_OTHER, &sp);
	prnt_policy(0);
	prnt_priority(0);
	puts("");


	puts("nice -10");
	nice(-10);
	prnt_priority(0);
	puts("");
	

	puts("nice 20");
	nice(20);
	prnt_priority(0);
	puts("");
	

	printf("setpriority 50\n");
	setpriority(PRIO_PROCESS, getpid(), 50);
	prnt_priority(0);
	puts("");


	puts("Main thread: switch policy to SCHED_RR with max priority");
	sp.sched_priority = sched_get_priority_max(SCHED_RR);
	sched_setscheduler(0, SCHED_RR, &sp);
	prnt_policy(0);
	prnt_priority(0);
	puts("");
	

	printf("setpriority 10\n");
	setpriority(PRIO_PROCESS, getpid(), 10);
	prnt_priority(0);
	puts("");	


	printf("nice -10\n");
	nice(-10);
	prnt_priority(0);
}


