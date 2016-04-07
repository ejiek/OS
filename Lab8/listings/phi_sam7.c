#include <cstdlib>
#include <stdio.h> 
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <iostream>
#define DEF_CONFIGFILE "Config.txt"
#define LOG_FILE "logFileSam7.txt"
#define DEF_KEY "key"
#define NUMBER_OF_FIL 5

struct Configuration
{
	int numOfReaders;
	int numOfWriters;
	int sizeOfQueue; 
	int readersDelay;
	int writersDelay; 
	int ttl; 
};

struct Node
{
    int value;
};

void CreateAllThreads(struct Configuration * config);
void SetConfig(char * filename, struct Configuration * config);
void* ThreadTimeManagerHandler(void* prm);
void* ThreadWriterHandler(void* prm);

struct Configuration config;
bool isDone = false;
pthread_t *allhandlers;
char shareFileName[] = "$$MyVerySpecialShareFileName$$";
int msgnum = 0;
pthread_mutex_t m_fork[NUMBER_OF_FIL];
int s_waiter;
static struct sembuf semLock[1] = {0,-1,0};
static struct sembuf semUnLock[1] = {0,1,0};
static struct sembuf semUnLockAll[1] = {0,4,0};
bool toResumeThreads=false;
FILE* logFile;


int main(int argc, char* argv[])
{
	char filename[100];
	if (argc<2)
	{
		strcpy(filename, DEF_CONFIGFILE);
	}
	else
	{
		strcpy(filename, argv[1]);
	}
	logFile = fopen(LOG_FILE, "w");

	SetConfig(filename, &config);
	for (int i = 0; i < NUMBER_OF_FIL; i++)
	{
		//m_fork[i] = CreateMutex(&sa1, false, NULL);                
                m_fork[i] = PTHREAD_MUTEX_INITIALIZER;
	}
        
	//s_waiter = CreateSemaphore(&sa1, NUMBER_OF_FIL-1, NUMBER_OF_FIL-1, NULL);
        char keyFile[100];
        bzero(keyFile,100);
        strcpy(keyFile,DEF_KEY);
        key_t key;
        if((key = ftok(keyFile, 'Q')) < 0) 
        {
            printf("Can't get key for key file %s and id 'Q'\n",keyFile);
            exit(1);
        }
        if((s_waiter = semget(key, 1, IPC_CREAT | 0666)) < 0)
        {
            printf("Error while creating semaphore\n");
            exit(2);
        }
        // устнавливаем 2 семафор в 1, т.е. можно писать
        if(semop(s_waiter, semUnLockAll, 1) < 0) {
        printf("execution complete\n");
        exit(1);//kill(getpid(),SIGINT);
        }
        ///

	CreateAllThreads(&config);
        
        toResumeThreads=true;
        
        for (int i = 0; i<config.numOfWriters + 1; i++)
		pthread_join(allhandlers[i],NULL);
                
	if(semctl(s_waiter, 0, IPC_RMID) < 0) 
        {
            printf("Error while deleting semaphore\n");
            exit(1);
        }
        delete []allhandlers;
	printf("all is done\n");
	fclose(logFile);        
        std::cout << "Press enter to continue...";
        std::cin.ignore(1024, '\n');
	return 0;
}
