#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


void handler() {
	puts("Interruption catched!");
	pthread_exit(0);
}
	
void* thread1() {
	while(1) {
		puts("Thread 1");
		sleep(1);
	}
}

pthread_t t1;

void main() {
	signal(SIGUSR2, handler);
	pthread_create(&t1, NULL, thread1, NULL);
	sleep(1);
	pthread_kill(t1, SIGUSR2);
	while(1) {
		puts("Main Thread");
		sleep(1);
	}
}

