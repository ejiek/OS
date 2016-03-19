#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <string.h>
#include "shm.h"

Message* p_msg;
int shmemory;
int semaphore;

void intHandler(int sig) {
	//turn off shared memmory
	if(shmdt(p_msg) < 0) {
		printf("Error while detaching shm\n");
		exit(1);
	}
	//delete shm and semaphores
	if(shmctl(shmemory, IPC_RMID, 0) < 0) {
		printf("Error while deleting shm\n");
		exit(1);
	}
	if(semctl(semaphore, 0, IPC_RMID) < 0) {
		printf("Error while deleting semaphore\n");
		exit(1);
	}
}
int main(int argc, char** argv) {
	char keyFile[100];
	bzero(keyFile,100);
	if(argc < 2) {
		printf("Using default key file %s\n",DEF_KEY_FILE);
		strcpy(keyFile,DEF_KEY_FILE);
	} else
		strcpy(keyFile,argv[1]);
	key_t key;
	// uses same key for semaphore and shm
	if((key = ftok(keyFile, 'Q')) < 0) {
		printf("Can't get key for key file %s and id 'Q'\n",keyFile);
		exit(1);
	}
	//create shm
	if((shmemory = shmget(key, sizeof(Message), IPC_CREAT | 0666)) < 0) {
		printf("Can't create shm\n");
		exit(1);
	}
	//connect shm to ower adress space
	if((p_msg = (Message*)shmat(shmemory, 0, 0)) < 0) {
		printf("Error while attaching shm\n");
		exit(1);
	}
	// sets signal wrapper
	signal(SIGINT, intHandler);
	//create group of 2 semaphores
	//1 - shows possibility to read
	//2 - shows possibility to write
	if((semaphore = semget(key, 2, IPC_CREAT | 0666)) < 0) {
		printf("Error while creating semaphore\n");
		kill(getpid(),SIGINT);
	}
	//  sets semaphore 2 to 1, we can write
	if(semop(semaphore, setWriteEna, 1) < 0) {
		printf("execution complete\n");
		kill(getpid(),SIGINT);
	}
	// main loop
	for(;;) {
		// waits for client
		if(semop(semaphore, readEna, 1) < 0) {
			printf("execution complete\n");
			kill(getpid(),SIGINT);
		}
		//read clients message
		printf("Client's message: %s", p_msg->buf);
		// tell client that he can write again
		if(semop(semaphore, setWriteEna, 1) < 0) {
			printf("execution complete\n");
			kill(getpid(),SIGINT);
		}
	}
}

