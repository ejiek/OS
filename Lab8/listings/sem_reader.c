#include <cstdlib>
#include <stdio.h> 
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#define DEF_KEY "//home//user//NetBeansProjects//sam3//key"
#define BUF_SIZE 5 //sizeOfQueue
int semaphore;
int* buf;
int FileMapping;
bool isDone=false;
static struct sembuf canWriteLock[1] = {0,-1,0};
static struct sembuf canWriteUnLock[1] = {0,1,0};
static struct sembuf canReadLock[1] = {1,-1,0};
static struct sembuf canReadUnLock[1] = {1,1,0};


void intHandler(int sig) {
    isDone = true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("error with start reader process. argc=%d Need 1 arguments\n",
                        argc);
		system("pause");
		exit(1000);
	}
        
        signal(SIGUSR1,intHandler);
	int id = atoi(argv[1]);
        printf("Process id:%d started\n",id);

	//HANDLE semaphore = (HANDLE)atoi(argv[2]);
        char keyFile[100];
        bzero(keyFile,100);
        strcpy(keyFile,DEF_KEY);
        key_t key;
        if((key = ftok(keyFile, 'Q')) < 0) 
        {
            printf("Can't get key for key file %s and id 'Q'\n",keyFile);
            exit(1);
        }
        if((semaphore = semget(key, 2, IPC_CREAT | 0666)) < 0)
        {
            printf("Error while creating semaphore\n");
            exit(2);
        }
        ///
        
        //создаем shm
        if((FileMapping = shmget(key, (BUF_SIZE+3)*sizeof(int), 0666)) < 0) {
        printf("Can't create shm\n");
        exit(1);
        }
        //присоединяем shm в наше адресное пространство
        if((buf = (int*)shmat(FileMapping, 0, 0)) < 0) {
        printf("Error while attaching shm\n");
        exit(1);
        }

	while (isDone!=true)
	{
		//WaitForSingleObject(semaphore, INFINITE);
                //Захватываем семафовр на чтение
                if(semop(semaphore, canReadLock, 1) < 0) {
                    printf("execution complete\n");
                    exit(4);
                }
                
		printf("Reader id:%d read msg \"%s\"\n",id, ((char *)buf) + sizeof(int) * 2);			
		
		(*(((int *)buf) + 1))--;
                
		int i = *((int *)buf+1);
		printf("i:%d\n", i);
                //Ждем пока не прочитают все
		while ((*(((int *)buf) + 1)) != 0)
		{
                    sleep(0);
                }
		//printf("Proc finish read\n");
		if (i == 0)
		{
			//printf("ReleasMutex return %d\n",ReleaseMutex(semaphore));
			//ReleaseSemaphore(semaphore,1,NULL);
                        if(semop(semaphore, canWriteUnLock, 1) < 0) {
                            printf("execution complete\n");
                            exit(4);
                        }
			//printf("ReleaseMutex semaphore\n");
		}
	}
//	UnmapViewOfFile(lpFileMapForReaders);
//	CloseHandle(hFileMapping);          
	shmdt(buf);
        shmctl(FileMapping, IPC_RMID, 0);
	printf("all is done\n");
	getchar();
	return 0;
}
