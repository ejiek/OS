#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_t t1,t2, t3, t4;
struct sched_param sp;


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

	printf("%s: %s\n", author, info);
}


void prnt_priority(int thread_number){

}


void* thread1() {
	prnt_policy(1);
	sleep(3);
	prnt_policy(1);
}


void* thread2() { 
	prnt_policy(2);
	puts("Thread 2: change policy to SCHED_RR");
	sp.sched_priority = sched_get_priority_min(SCHED_RR);
	sched_setscheduler(0, SCHED_RR, &sp);
	prnt_policy(2);
	
}


void* thread3() {
	puts("Thread 3: change policy to SCHED_OTHER by group id");
	sp.sched_priority = sched_get_priority_min(SCHED_OTHER);
	sched_setscheduler(getpid(), SCHED_OTHER, &sp);
	prnt_policy(3);
}

void* thread4() {
	prnt_policy(4);
	sleep(9);
	prnt_policy(4);
}

void main() {
	prnt_policy(0);
	puts("Main thread: create thread 1");
	pthread_create(&t1, NULL, thread1, NULL);
	sleep(1);

	puts("Main thread: switch policy to SCHED_FIFO");
	sp.sched_priority = sched_get_priority_min(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sp);
	prnt_policy(0);
	pthread_join(t1, NULL);
	puts("");


	puts("Main thread: create thread 2");
	prnt_policy(0);
	pthread_create(&t2, NULL, thread2, NULL);
	pthread_join(t2, NULL);
	prnt_policy(0);
	puts("");


	puts("Main thread: create thread 3");
	prnt_policy(0);
	pthread_create(&t3, NULL, thread3, NULL);
	pthread_join(t3, NULL);
	prnt_policy(0);
	puts("");

	pthread_create(&t4, NULL, thread4, NULL);
	prnt_policy(0);
	sleep(1);
	puts("waiting for other procces to change my policy");
	printf("My PID: %d\n", getpid());
	sleep(9);
	prnt_policy(0);
}

