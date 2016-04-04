#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 
#include "unistd.h" 
#include "sys/signal.h" 

int main(int argc, char * argv[]){ 
	union sigval info_to_send; // структура, оправляемая с сигналом

	char huge_string[8193];
	memset (huge_string,'_',8192);
	memset (huge_string+8192,'\0',1);
	info_to_send.sival_int = 1;
	info_to_send.sival_ptr = &huge_string;
	sigqueue(atoi(argv[1]), SIGRTMIN, info_to_send);
	printf("signal sent to %d\n", atoi(argv[1]));
	sleep(60);
	return 0; 
}
