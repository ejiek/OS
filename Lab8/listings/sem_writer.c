void* ThreadWriterHandler(void* prm)
{
    struct Node *arg = prm;
	int myid = arg->value;
        
    while(!toResumeThreads)
        sleep(0);
    while (isDone != true)
    {
        //EnterCriticalSection(&crs);
        //WaitForSingleObject(semaphore, INFINITE);
        //Захватываем мьютекс на запись (Вроде пашет)
        if(semop(semaphore, canWriteLock, 1) < 0) {
            printf("execution complete\n");
            exit(4);
        }
        msgnum++; 
        sprintf(((char *)buf) + sizeof(int) * 2, "writer_id %d, msg with num = %d", myid, msgnum);
        printf("writer put msg: \"%s\" \n", ((char *)buf) + sizeof(int) * 2);
        *(((int *)buf) + 1) = config.numOfReaders;
        //ReleaseSemaphore(semaphore,config.numOfReaders,NULL);
        //Разрешаем читать - освобождаем все семафоры (3шт)
        if(semop(semaphore, canReadUnLockAll, 1) < 0) {
            printf("execution complete\n");
            exit(4);
        }
    }
    return 0;
}
