#include <stdio.h>
#include <unistd.h>

int main() {
        int pid, ppid;
        pid = getpid();
        ppid = getppid();
        printf("\nSon param: pid = %d, ppid = %d\n", pid, ppid);
        sleep(5);
        return 0;
}
