void* ThreadWriterHandler(void* prm)
{
    struct Node *arg = prm;
	int myid = arg->value;
        
    while(!toResumeThreads)
        sleep(0);
    int leftFork = myid+1;
    int rightFork = myid; 
    int numOfEats = 0;
    if (myid == NUMBER_OF_FIL)
    {
        leftFork = 0;
    }
    while (isDone != true)
    {
        printf("philosopher %d: thinks\n", myid);
        fprintf(logFile,"philosopher %d: thinks\n", myid);
        //Sleep(100);	
        usleep(100000);
        //WaitForSingleObject(s_waiter, INFINITE);
        if(semop(s_waiter,semLock , 1) < 0) {
            printf("execution complete\n");
            exit(3);
        }
        printf("philosopher %d: allowed to eat\n", myid);
        fprintf(logFile,"philosopher %d: allowed to eat\n", myid);
        //WaitForSingleObject(m_fork[rightFork], INFINITE);
        pthread_mutex_lock(&m_fork[rightFork]);
        printf("philosopher %d: took the right fork\n", myid);
        fprintf(logFile,"philosopher %d: took the right fork\n", myid);
        //WaitForSingleObject(m_fork[leftFork], INFINITE);
        pthread_mutex_lock(&m_fork[leftFork]);
        printf("philosopher %d: took the left fork\n", myid);
        fprintf(logFile,"philosopher %d: took the left fork\n", myid);
        numOfEats++;
        printf("philosopher %d: EATS %d times\n", myid, numOfEats);
        fprintf(logFile,"philosopher %d: EATS %d times\n", myid, numOfEats);
        //Sleep(100);
        usleep(100000);
        printf("philosopher %d: put the right fork\n", myid);
        fprintf(logFile,"philosopher %d: put the right fork\n", myid);
        //ReleaseMutex(m_fork[rightFork]);
        pthread_mutex_unlock(&m_fork[rightFork]);    
        printf("philosopher %d: put the left fork\n", myid);
        fprintf(logFile,"philosopher %d: put the left fork\n", myid);
        //ReleaseMutex(m_fork[leftFork]);	
        pthread_mutex_unlock(&m_fork[leftFork]);  
        //ReleaseSemaphore(s_waiter, 1, NULL);
        if(semop(s_waiter, semUnLock, 1) < 0)
        {
            printf("execution complete\n");
            exit(4);
        }
    }
    delete arg;
    return 0;
}
