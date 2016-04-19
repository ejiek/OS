#include "onecore.h"

int one_core(int pid){
	cpu_set_t set;

	CPU_ZERO( &set );
	CPU_SET( 0, &set );
	if (sched_setaffinity( pid, sizeof( cpu_set_t ), &set )){
		perror( "sched_setaffinity" );
		return -1;
	}
	return 0;
}

