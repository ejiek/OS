int main(int argc, char* argv[])
{
	char filename[100];
	if (argc<2)
	{
		strcpy(filename, DEF_CONFIGFILE); //Конфигурация по умолчанию
	}
	else
	{
		strcpy(filename, argv[1]); //Конфигурирование входными параметрами
	}
        
//значения TYPE_OF_SYNCHRONISE перечислены выше
	#if TYPE_OF_SYNCHRONISE == 0
		strcpy(logFileName, "logMUTEX.txt");
	#endif
	#if TYPE_OF_SYNCHRONISE == 1
		strcpy(logFileName, "logSEM.txt");
	#endif
	#if TYPE_OF_SYNCHRONISE == 2
		strcpy(logFileName, "logCRS.txt");
	#endif
	#if TYPE_OF_SYNCHRONISE == 3
		strcpy(logFileName, "logEVENT.txt");
	#endif
	#if TYPE_OF_SYNCHRONISE == 4
		strcpy(logFileName, "logCOND.txt");
	#endif

	logFile = fopen(logFileName, "w");

	SetConfig(filename, &config);
	CreateAllThreads(&config);
	queue.full = 0;
	queue.readindex = 0;
	queue.writeindex = 0;
	queue.size = config.sizeOfQueue;
	queue.data = new char*[config.sizeOfQueue];

	#if TYPE_OF_SYNCHRONISE == 0
	//mutex = CreateMutex(NULL, FALSE, "");
        
        #endif
	#if TYPE_OF_SYNCHRONISE == 1
        char keyFile[100];
        bzero(keyFile,100);
        strcpy(keyFile,DEF_KEY);
        key_t key;
        if((key = ftok(keyFile, 'Q')) < 0) 
        {
            printf("Can't get key for key file %s and id 'Q'\n",keyFile);
            exit(1);
        }
        if((semaphore = semget(key, 1, IPC_CREAT | 0666)) < 0)
        {
            printf("Error while creating semaphore\n");
            exit(2);
        }
        // устнавливаем 2 семафор в 1, т.е. можно писать
        if(semop(semaphore, semUnLock, 1) < 0) {
        printf("execution complete\n");
        kill(getpid(),SIGINT);
        }
	//sem = CreateSemaphore(NULL, 1, 1, "");
	#endif
	#if TYPE_OF_SYNCHRONISE == 2
	InitializeCriticalSection(&crs);
	#endif
	#if TYPE_OF_SYNCHRONISE == 3
	//event = CreateEvent(NULL, false, true, "");
            sigemptyset(&newset);   //список пуст
            sigaddset(&newset, SIGUSR1); //добавление сигнлала в список
            pthread_sigmask(SIG_BLOCK, &newset, NULL);
	#endif
	#if TYPE_OF_SYNCHRONISE == 4
	InitializeCriticalSection(&crs2);
	InitializeConditionVariable(&condread);
	InitializeConditionVariable(&condwrite);
	#endif
	//for (int i = 0; i<config.numOfReaders + config.numOfWriters + 1; i++)
	//	ResumeThread(allhandlers[i]);
        toResumeThreads=true;
#if TYPE_OF_SYNCHRONISE == 3
        kill(getpid(), SIGUSR1);
#endif
        
#if TYPE_OF_SYNCHRONISE == 4
	WaitForMultipleObjects(config.numOfReaders+config.numOfWriters+1,allhandlers,TRUE,5000);
#else
	//WaitForMultipleObjects(config.numOfReaders + config.numOfWriters + 1, allhandlers, TRUE, INFINITE);
        for (int i = 0; i<config.numOfReaders + config.numOfWriters + 1; i++)
		pthread_join(allhandlers[i],NULL);
#endif
	//for (int i = 0; i<config.numOfReaders + config.numOfWriters + 1; i++) 
	//	CloseHandle(allhandlers[i]);

	#if TYPE_OF_SYNCHRONISE == 0
		//CloseHandle(mutex);
                
	#endif
	#if TYPE_OF_SYNCHRONISE == 1
            if(semctl(semaphore, 0, IPC_RMID) < 0) {
            printf("Error while deleting semaphore\n");
            exit(1);
            }
	//	CloseHandle(sem);
	#endif
	#if TYPE_OF_SYNCHRONISE == 2
		DeleteCriticalSection(&crs);
	#endif
	#if TYPE_OF_SYNCHRONISE == 3
		//CloseHandle(event);
	#endif
	#if TYPE_OF_SYNCHRONISE == 4
		DeleteCriticalSection(&crs2);
	#endif

        delete []queue.data;
        delete []allhandlers;
	printf("all is done\n");
	fprintf(logFile, "all is done\n");
	fclose(logFile);
        
        std::cout << "Press enter to continue...";
        std::cin.ignore(1024, '\n');
        //std::cin.get();
	return 0;
}
