#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argc, char ** argv){
    int fd;
    ssize_t ret;
    char ch;
    if (argc < 2){
        fprintf (stderr, "You have to provide a filename\n");
        return 1;
    }

    fd = open (argv[1], O_RDONLY);
    if (fd < 0){
        fprintf (stderr, "Cannot open file\n");
        return 1;
    }	
	
    while ((ret = read (fd, &ch, 1)) > 0){
        putchar (ch);
    }

    if (ret < 0){
        fprintf (stderr, "Error while reading: Cannot read file\n");
        return 1;
    }
    close (fd);
    return 0;
}
