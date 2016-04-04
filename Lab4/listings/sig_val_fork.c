#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 
#include "unistd.h" 
#include "sys/signal.h" 

// Обработчик сигналов
void sighandler(int signo, siginfo_t *siginfo) { 
	return; 
}

int main(int argc, char * argv[]){ 
	sigset_t mask;	// сигналы, которые следует блокировать 
	int sa_flags; // необходимые флаги
	struct sigaction act; // обеспечивает детализированный контроль над сигналами
	siginfo_t localinfo;

	memset(&act, 0, sizeof(act)); 
	sigemptyset(&mask);

	sigaddset(&mask, SIGRTMIN); 

	sa_flags = SA_SIGINFO;

	act.sa_handler = sighandler;	     	// обработчик сигнала или действие
	act.sa_mask = mask; 			// сигналы, которые следует блокировать
	act.sa_flags = sa_flags;

	sigaction(SIGRTMIN, 	&act, NULL); 

	char huge_string[8193];
	memset (huge_string,'_',8192);
	memset (huge_string+8192,'\0',1);
	printf("original filler: '%c'\n", huge_string[0]);	

	if(fork() == 0){
		memset (huge_string,'+',8192);
		printf("son changes filler to '%c'\n", huge_string[0]);
		union sigval info_to_send; // структура, оправляемая с сигналом
	        info_to_send.sival_ptr = &huge_string;
		sigqueue(getppid(), SIGRTMIN, info_to_send);
		sleep(2);
	}
	else{
		sigwaitinfo(&mask, &localinfo);
		char *localstring;
		localstring = (char*)localinfo.si_value.sival_ptr;
		printf("filler of the pointed string '%c'\n", localstring[0]);
	}
	return 0; 
}
