#include "stdio.h" 
#include "stdlib.h" 
#include "string.h" 
#include "unistd.h" 
#include "sys/signal.h" 

/* Максимальное число сигналов, которые можно принять */ 
#define MAX_SENDED_SIGS 15 

/* Массив, в который записываются номера принятых сигналов */ 
int rec_sig[MAX_SENDED_SIGS]; 

/* Следующий свободный для записи элемент в массиве */ 
unsigned int nfree_elem = 0; 

/* обработчик сигнала заносит номер принятого сигнала в массив */
void sighandler(int signo) 
{ 
	rec_sig[nfree_elem++] = signo; 
	return; 
} 

int main(int argc, char * argv[])
{ 
	sigset_t mask;	// сигналы, которые следует блокировать 
	struct sigaction act; // обеспечивает детализированный контроль над сигналами
	int i; 

	memset(&act, 0, sizeof(act)); 
	/* инициализирует набор сигналов, указанный в set, поме­чая его
	* как пустое множество (из этого набора исключаются все сигналы) 
	*/
	sigemptyset(&mask);

	/* Добавляем в маску те сигналы, которые мы будем  
	* отправлять/принимать 
	*/ 
	sigaddset(&mask, SIGRTMIN); 
	sigaddset(&mask, SIGRTMIN + 1);
	sigaddset(&mask, SIGRTMIN + 3);  
	sigaddset(&mask, SIGUSR1); 
	sigaddset(&mask, SIGUSR2);
	/* Устанавливаем обработчик для наших сигналов. 
	* Добавляем наши сигналы в список блокируемых при вызове  
	* обработчика сигналов – во избежание гонок. 
	*/ 
	act.sa_handler = sighandler;	     	/* обработчик сигнала или действие */ 
	act.sa_mask = mask; 			/* сигналы, которые следует блокировать */
	/* Вызов функции sigaction() изменяет поведение сигнала, обозначенного аргументом 
	* signo. Этот аргумент может принимать любые значения, кроме тех, что
	* ассоциируются с SIGKILL и SIGSTOP. Если act не равно NULL, то этот системный вызов
	* изменяет актуальное поведение сигнала так, как это указано в act. Если oldact не
	* равно NULL, то вызов сохраняет там предшествующее (а в случае NULL  — актуальное)
	* поведение указанного сигнала.
	*/
	sigaction(SIGUSR1,  	&act, NULL);
	sigaction(SIGUSR2,  	&act, NULL);
	sigaction(SIGRTMIN, 	&act, NULL); 
	sigaction(SIGRTMIN + 1, &act, NULL); 
	sigaction(SIGRTMIN + 3, &act, NULL); 
 
	/* Поведение sigprocmask() зависит от значения how, в качестве которого может
	* служить один из следующих флагов:
	* - SIG_SETMASK  — сигнальная маска вызывающего процесса изменяется на set;
	* - SIG_BLOCK  — сигналы set добавляются к сигнальной маске вызывающего процес­са. 
	* Иными словами, сигнальная маска заменяется объединением (двоичным «ИЛИ») из 
	* актуальной маски и set;
	* - SIG_UNBLOCK  — сигналы set удаляются из сигнальной маски вызывающего процесса. 
	* Иными словами, сигнал заменяется пересечением (двоичным «НЕ») set.
	*/

	/* Блокируем доставку наших сигналов, чтобы избежать их  
	* немедленной доставки 
	*/ 
	sigprocmask(SIG_BLOCK, &mask, NULL); 

	/* Отправляем сигналы (не забываем о блокировке строчкой выше) */ 
	/* с помощью функции raise() процесс легко может отправить сигнал сам себе */
	raise(SIGUSR1);
	raise(SIGRTMIN + 3); 
	raise(SIGRTMIN + 1); 
	raise(SIGRTMIN); 
	raise(SIGUSR2);
	raise(SIGRTMIN + 1); 
	raise(SIGRTMIN); 
	raise(SIGRTMIN + 3); 
	raise(SIGUSR1); 
	raise(SIGUSR1); 

	/* Разблокируем сигналы – все сигналы приходят одновременно */ 
	sigprocmask(SIG_UNBLOCK, &mask, NULL); 

	/* Выводим на экран список принятых сигналов в порядке их поступления */ 
	for(i = 0; i < nfree_elem; ++i) 
	{ 
		if (rec_sig[i] == SIGUSR1) 
		{ 
			printf("We get SIGUSR1\n"); 
		} 
		else if (rec_sig[i] == SIGUSR2) 
		{ 
			printf("We get SIGUSR2\n"); 
		} 
		else if (rec_sig[i] == SIGRTMIN) 
		{ 
			printf("We get SIGRTMIN\n"); 
		} 
		else if (rec_sig[i] == SIGRTMIN + 1) 
		{ 
			printf("We get SIGRTMIN + 1\n"); 
		} 
		else if (rec_sig[i] == SIGRTMIN + 3) 
		{ 
			printf("We get SIGRTMIN + 3\n"); 
		} 
	} 
	return 0; 
}
