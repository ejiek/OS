#include <stdio.h>
#include <sched.h> 

int main (void) 
{
	struct sched_param shdprm;
	printf ("SCHED_FIFO : from %d to %d\n",
	sched_get_priority_min (SCHED_FIFO), sched_get_priority_max (SCHED_FIFO)); 
	printf ("SCHED_RR : from %d to %d\n",
	sched_get_priority_min (SCHED_RR), sched_get_priority_max (SCHED_RR)); 
	printf ("SCHED_OTHER: from %d to %d\n",
	sched_get_priority_min (SCHED_OTHER), sched_get_priority_max (SCHED_OTHER));
	printf ("Current policy for this proccess: "); 
	switch (sched_getscheduler (0)) 
	{ 
		case SCHED_FIFO:
		printf ("SCHED__FIFO\n"); break;
		case SCHED_RR:
		printf ("SCHED_RR\n"); break;
		case SCHED_OTHER:
		printf ("SCHED_OTHER\n"); break; case -1:
		perror ("SCHED_GETSCHEDULER"); break; default:
		printf ("Unknown policy\n");
	}

	if (sched_getparam (0, &shdprm) == 0) 
	{
		printf ("Current priority for this proccess: %d\n", shdprm.sched_priority);
	} 
	else 
	{
		perror ("SCHED_GETPARAM");
	}

return 0;
}
