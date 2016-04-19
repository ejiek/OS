#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

void* thread1() {
	int i = 0;
	while(1) {
		printf("thread1, %d\n", i++);
		sleep(1);
	}
}

void* thread2() {
	int i = 0;
	while(1) {
		printf("thread2, %d\n", i++);
		sleep(5);
	}
}
pthread_t t1,t2;

void main() {
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);

	sleep(5);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

}
