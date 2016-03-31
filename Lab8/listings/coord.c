void* ThreadTimeManagerHandler(void* prm)
{
	int ttl = *((int*)(prm));
	if (ttl<0)
	{
		char buf[100];
		while (1)
		{
			fgets(buf, sizeof(buf), stdin);
			if (buf[0] == 's');
			{
				isDone = true;
				break;
			}
		}
	}
	else
	{
            sleep(ttl);
            isDone = true;
	}
	printf("TimeManager finishing work\n");
	fprintf(logFile,"TimeManager finishing work\n");
	return 0;
}
