#include <stdio.h>

void main(){
    char storage[1000];
    fgets(storage, sizeof(storage)-2, stdin);
    printf("Right output: %s",storage);
}
