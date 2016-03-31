void* ThreadWriterHandler(void* prm)
{
    while(!toResumeThreads)
        sleep(0);
    struct Node *arg = prm;
	int myid = arg->value;//*((int*)(prm));
	char tmp[50];
	int msgnum = 0;
	while (isDone != true)
	{
		#if TYPE_OF_SYNCHRONISE == 0
                    pthread_mutex_lock(&mutex);	
                    //WaitForSingleObject(mutex, INFINITE);
		#endif
		#if TYPE_OF_SYNCHRONISE == 1
                    if(semop(semaphore,semLock , 1) < 0) {
                        printf("execution complete\n");
                        exit(5);
                    }
		//	WaitForSingleObject(sem, INFINITE);
		#endif
		#if TYPE_OF_SYNCHRONISE == 2
			EnterCriticalSection(&crs);
		#endif
		#if TYPE_OF_SYNCHRONISE == 3
                            //WaitForSingleObject(event, INFINITE);
                    int sig; //значение прерывающего сигнлада
                     //Перед тем как вызывать sigwait(),
                    //набор ожидаемых сигналов следует заблокировать
                    //с помощью функции sigprocmask(), иначе,
                    //при получении сигнала, вместо выхода из sigwait()
                    //будет вызван соответствующий обработчик.
                    //sigprocmask(SIG_BLOCK, &newset, 0);
                    if(sigwait(&newset, &sig)!=0)
                        printf("eroro in whait lock signal\n");
                    else
                        printf("get sig %d\n",sig);
		#endif
		#if TYPE_OF_SYNCHRONISE == 4
			EnterCriticalSection(&crs2); 
			while (!(queue.readindex != queue.writeindex || !queue.full == 1)) 
				SleepConditionVariableCS(&condwrite,&crs2,INFINITE);
		#endif
		
                        if (queue.readindex != queue.writeindex || !queue.full == 1)
		{
			sprintf(tmp, "writer_id = %d numMsg= %3d", myid, msgnum);
			queue.data[queue.writeindex] = strdup(tmp);
			msgnum++;
			printf("Writer %d put data: \"%s\" in position %d\n", myid, queue.data[queue.writeindex], queue.writeindex);
			fprintf(logFile,"Writer %d put data: \"%s\" in position %d\n", myid, queue.data[queue.writeindex], queue.writeindex); 
			queue.writeindex = (queue.writeindex + 1) % queue.size;
			queue.full = queue.writeindex == queue.readindex ? 1 : 0; 
		}
                        
		#if TYPE_OF_SYNCHRONISE == 0
                    pthread_mutex_unlock(&mutex);
				//ReleaseMutex(mutex);
		#endif
		#if TYPE_OF_SYNCHRONISE == 1
                    if(semop(semaphore, semUnLock, 1) < 0) {
                    printf("execution complete\n");
                    exit(6);
                    }
		//		ReleaseSemaphore(sem, 1, NULL);
		#endif
		#if TYPE_OF_SYNCHRONISE == 2
				LeaveCriticalSection(&crs);
		#endif
		#if TYPE_OF_SYNCHRONISE == 3
				//SetEvent(event);
                                kill(getpid(), sig);
		#endif
		#if TYPE_OF_SYNCHRONISE == 4
				if (queue.full == 1)
				{
					printf("queue is full\n"); 
					fprintf(logFile, "queue is full\n"); 
				}
				WakeConditionVariable(&condread);
				LeaveCriticalSection(&crs2);
		#endif
		sleep(config.writersDelay);
	}
	printf("Writer %d finishing work\n", myid);
	fprintf(logFile,"Writer %d finishing work\n", myid);
        delete arg;
	return 0;
}
