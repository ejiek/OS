#include <stdio.h>

void main(){
    char storage[1000];
    fgets(storage, sizeof(storage), stdin);
    printf("Right output: %s",storage);
}
