#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


void handler() {
	puts("Interruption catched!");
	pthread_exit(0);
}
	
void* thread1() {
	signal(SIGUSR2, handler);
	sleep(1);
	kill(getpid(), SIGUSR2);
	while(1) {
		puts("Thread 1");
		sleep(1);
	}
}

pthread_t t1;

void main() {
	pthread_create(&t1, NULL, thread1, NULL);
	while(1) {
		puts("Main Thread");
		sleep(1);
	}
}

