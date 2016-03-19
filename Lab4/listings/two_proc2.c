#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Signal SIGUSR1 sent. Current time and date: %s", asctime (timeinfo) );	
	kill(atoi(argv[1]),SIGUSR1);
}

