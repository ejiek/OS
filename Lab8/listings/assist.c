void CreateAllThreads(struct Configuration * config) 
{
	printf("createConfig:\n NumOfreadrs = %d | ReadersDelay= %d | NumOfwriters= %d | WritersDelay = %d | sizeofqueue = %d | ttl = %d\n", config->numOfReaders, config->readersDelay, config->numOfWriters, config->writersDelay, config->sizeOfQueue, config->ttl);
	fprintf(logFile,"createConfig:\n NumOfreadrs = %d | ReadersDelay= %d | NumOfwriters= %d | WritersDelay = %d | sizeofqueue = %d | ttl = %d\n", config->numOfReaders, config->readersDelay, config->numOfWriters, config->writersDelay, config->sizeOfQueue, config->ttl);
	allhandlers = new pthread_t[config->numOfReaders + config->numOfWriters + 1];
	int count = 0;
	printf("create readers\n");
	fprintf(logFile,"create readers\n");
        Node* arg;
	for (int i = 0; i <= config->numOfReaders - 1; i++, count++)
	{
            arg=new Node();
            arg->value = i;
		printf("count= %d\n", count);
		fprintf(logFile,"count= %d\n", count);
		//if ((allhandlers[count] = CreateThread(NULL, 0, ThreadReaderHandler, (LPVOID)i, CREATE_SUSPENDED, NULL)) == NULL) 
                if (pthread_create(&allhandlers[count], NULL, ThreadReaderHandler, arg) != 0) 
		{
			printf("impossible to create thread-reader\n %d", errno);
			fprintf(logFile,"impossible to create thread-reader\n %d", errno);
			//exit(8000);
                        return;
		}
	}
	printf("create writers\n");
	fprintf(logFile,"create writers\n");
	for (int i = 0; i <= config->numOfWriters - 1; i++, count++) 
	{
            arg = new Node();
            arg->value=i;
		printf("count= %d\n", count);
		fprintf(logFile,"count= %d\n", count);
		if (pthread_create(&allhandlers[count], NULL, ThreadWriterHandler, arg) != 0) 
		{
			printf("impossible to create thread-writer\n");
			fprintf(logFile,"impossible to create thread-writer\n");
                        return;
		}
	}
	//TimeManager 
	printf("create TimeManager\n");
	fprintf(logFile,"create TimeManager\n");
	printf("count= %d\n", count);
	fprintf(logFile,"count= %d\n", count);
	if (pthread_create(&allhandlers[count], NULL, ThreadTimeManagerHandler, (void*)&config->ttl) != 0) 
	{
		printf("impossible to create thread-reader\n");
		fprintf(logFile,"impossible to create thread-reader\n");
                        return;
	}
	printf("successfully created threads\n");
	fprintf(logFile,"successfully created threads\n");
	return;
}

void SetConfig(char * filename, struct Configuration * config) 
{
	FILE *f;
	int numOfReaders;
	int numOfWriters;
	int readersDelay;
	int writersDelay;
	int sizeOfQueue;
	int ttl;
	char tmp[20];
	if ((f = fopen(filename, "r")) == NULL)
	{
		printf("impossible open config file %s\n", filename);
		fprintf(logFile,"impossible open config file %s\n", filename);
		return;
	}
	//NumOfReaders= 10 
	//ReadersDelay= 100 
	//NumOfWriters= 10 
	//WritersDelay= 200 
	//SizeOfQueue= 10 
	//ttl= 3 
	fscanf(f, "%s %d", tmp, &numOfReaders); 
	fscanf(f, "%s %d", tmp, &readersDelay); 
	fscanf(f, "%s %d", tmp, &numOfWriters); 
	fscanf(f, "%s %d", tmp, &writersDelay); 
	fscanf(f, "%s %d", tmp, &sizeOfQueue); 
	fscanf(f, "%s %d", tmp, &ttl);
	if (numOfReaders <= 0 || numOfWriters <= 0)
	{
		printf("incorrect num of Readers or writers\n");
		return;
	}
	else if (readersDelay <= 0 || writersDelay <= 0)
	{
		printf("incorrect delay of Readers or writers\n");
		return;
	}
	else if (sizeOfQueue <= 0)
	{
		printf("incorrect size of queue\n");
		return;
	}
	else if (ttl == 0)
	{
		printf("incorrect ttl\n");
		return;
	}
	config->numOfReaders = numOfReaders;
	config->readersDelay = readersDelay;
	config->numOfWriters = numOfWriters;
	config->writersDelay = writersDelay;
	config->sizeOfQueue = sizeOfQueue;
	config->ttl = ttl;
	return;
}
