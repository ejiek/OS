#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
        int pid, ppid, status;
        pid = getpid();
        ppid = getppid();
        printf("\nFATHER PARAM: pid = %d, ppid = %d\n\n", pid, ppid);
        if (fork() == 0) execl("son", "son", NULL);
        system("ps xf");
        wait(&status);
        return 0;
}
