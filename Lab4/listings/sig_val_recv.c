#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 
#include "unistd.h" 
#include "sys/signal.h" 

void printsig(int sig, int value){
	if (sig == SIGUSR1){ 
		printf("We get SIGUSR1\n"); 
	} 
	else if (sig == SIGUSR2){ 
		printf("We get SIGUSR2\n"); 
	} 
	else if (sig == SIGRTMIN){ 
		printf("We get SIGRTMIN | value :%d\n", value); 
	} 
	else if (sig == SIGRTMIN + 1){ 
		printf("We get SIGRTMIN + 1 | value :%d\n", value); 
	} 
	else if (sig == SIGRTMIN + 3){ 
		printf("We get SIGRTMIN + 3 | value :%d\n", value); 
	} 
}

// Обработчик сигналов
void sighandler(int signo, siginfo_t *siginfo) { 
	siginfo_t localinfo = *siginfo;
	printsig(signo,  localinfo.si_value.sival_int);
	return; 
} 

int main(int argc, char * argv[]){ 
	sigset_t mask;	// сигналы, которые следует блокировать 
	int sa_flags; // необходимые флаги
	struct sigaction act; // обеспечивает детализированный контроль над сигналами
	siginfo_t localinfo;

	//memset(&act, 0, sizeof(act)); 
	sigemptyset(&mask);

	sigaddset(&mask, SIGRTMIN); 

	sa_flags = SA_SIGINFO;

	act.sa_handler = sighandler;	     	/* обработчик сигнала или действие */ 
	act.sa_mask = mask; 			/* сигналы, которые следует блокировать */
	act.sa_flags = sa_flags;

	sigaction(SIGRTMIN, 	&act, NULL); 
	printf("PID: %d\n", getpid()); 
	sigwaitinfo(&mask, &localinfo);
	char *localstring;
	localstring = (char*)localinfo.si_value.sival_ptr;
	printf("info recived: %s", localstring);

	return 0; 
}
