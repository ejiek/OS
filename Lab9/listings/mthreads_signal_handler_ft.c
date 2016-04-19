#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t t1,t2;

void t2obr() {
	printf("signal in thread2\n");
	pthread_exit(NULL);
}

void* thread1() {
	printf("thread1 started\n");
	int i = 0;
	sleep(5);
	pthread_kill(t2, SIGUSR2);
	while(1) {
		printf("thread1, %d\n", i++);
		sleep(5);
	}
}

void* thread2() {
	signal(SIGUSR2, t2obr);
	printf("thread2 started\n");
	int i = 0;
	while(1) {
		printf("thread2, %d\n", i++);
		sleep(5);
	}
}

void main() {
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);

	sleep(5);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

}

