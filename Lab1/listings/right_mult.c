#include <stdio.h>
#include <string.h>

void main(){
    char storage[1000];
    do{ bzero(storage, strlen(storage));
        fgets(storage, sizeof(storage), stdin);
        if(storage[0]!='\0') printf("Right output: %s",storage);
    }while(storage[strlen(storage)-1]=='\n');
}
